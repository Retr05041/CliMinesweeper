#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
typedef unsigned int uint;

typedef struct{
    int x;
    int y;
}Coord;

typedef enum{G_PLAY, G_LOST, G_WIN, G_NUM}GameState;
const char *GameStateStr[G_NUM] = {"G_PLAY", "G_LOST", "G_WIN"};
typedef enum{TS_HIDE, TS_REVL, TS_FLAG, TS_QEST, TS_INVL, TS_NUM}TileState;
const char *TileStateStr[TS_NUM] = {"TS_HIDE", "TS_REVL", "TS_FLAG", "TS_QEST", "TS_INVL"};

typedef struct{
    TileState type;
    Coord pos;
}Action;

typedef struct{
    int num;    // -1 if tile is bomb
    TileState state;
}Tile;

typedef struct{
    GameState state;
    Coord len;
    uint numBombs;
    Tile **tile;
}Board;

void usage(void)
{
    fprintf(stderr, "Usage: ./main.out <grid width> <grid height> <num bombs>\n");
}

uint strUint(char *str)
{
    char *end = NULL;
    unsigned long ret = strtoul(str, &end, 0);
    if(errno == ERANGE || end != str+strlen(str)){
        fprintf(stderr, "Error parsing uint from: \"%s\"\n", str);
        usage();
        exit(EXIT_FAILURE);
    }
    return ret;
}

void boardPrintInfo(const Board board)
{
    printf("Board -\n");
    printf("\tlen.x: %u\n", board.len.x);
    printf("\tlen.y: %u\n", board.len.y);
    printf("\tnumBombs: %u\n", board.numBombs);
}

void printX(const int x)
{
    fputs("   |", stdout);
    for(int i = 0; i < x; i++)
        printf("%2u |", i);
    fputs("\n", stdout);
}

void printLine(const int x)
{
    for(int i = 0; i <= x; i++)
        fputs("---+", stdout);

    fputs("---\n", stdout);
}

void boardPrint(const Board board, const bool reveal)
{
    printX(board.len.x);
    printLine(board.len.x);
    for(int y = 0; y < board.len.y; y++){
        printf("%2u |", y);
        for(int x = 0; x < board.len.x; x++){
            switch(board.tile[x][y].state){
                case TS_HIDE:
                    if(reveal)
                        if(board.tile[x][y].num == -1)
                            fputs("[B]|", stdout);
                        else
                            printf("[%c]|", board.tile[x][y].num == 0 ? ' ' : '0'+board.tile[x][y].num);
                    else
                        fputs("[ ]|", stdout);
                    break;
                case TS_REVL:
                    if(board.tile[x][y].num == 0)
                        fputs("   |", stdout);
                    else if(board.tile[x][y].num == -1)
                        fputs("}#{|",  stdout);
                    else
                        printf("%2u |", board.tile[x][y].num);
                    break;
                case TS_FLAG:
                    fputs("[F]|", stdout);
                    break;
                case TS_QEST:
                    fputs("[?]|", stdout);
                    break;
                default:
                    fprintf(stderr, "board.tile[%u][%u].state is invalid(%i)\n", x, y, board.tile[x][y].state);
                    exit(EXIT_FAILURE);
            }
        }
        printf("%2u\n", y);
        printLine(board.len.x);
    }
    printX(board.len.x);
    fputs("\n", stdout);
}

Board boardParse(char** boardArgs)
{
    Board board = {
        .len = (Coord){
            .x = strUint(boardArgs[0]),
            .y = strUint(boardArgs[1])
        },
        .numBombs = strUint(boardArgs[2])
    };

    if(board.len.x < 4 || board.len.y < 4){
        fprintf(stderr, "board.board.len.x(%u) and board.len.y(%u) must be >= 4\n",
            board.len.x, board.len.y
        );
        exit(EXIT_FAILURE);
    }

    if((int)board.numBombs >= (board.len.x * board.len.y)-8){
        fprintf(stderr, "board.numBombs: %u\nmust be < %u\n(board.len.x(%u) * board.len.y(%u))\n",
            board.numBombs, board.len.x * board.len.y, board.len.x, board.len.y
        );
        exit(EXIT_FAILURE);
    }

    if(board.len.x > 99 || board.len.y > 99){
        fprintf(stderr, "board.board.len.x(%u) and board.len.y(%u) must be <= 99\n",
            board.len.x, board.len.y
        );
        exit(EXIT_FAILURE);
    }

    return board;
}

Board boardFree(Board board)
{
    if(board.tile){
        for(int x = 0; x < board.len.x; x++){
            if(board.tile[x])
                free(board.tile[x]);
        }
        free(board.tile);
    }
    return board;
}

Board boardAlloc(Board board)
{
    if(board.tile){
        for(int x = 0; x < board.len.x; x++){
            if(board.tile[x])
                free(board.tile[x]);
        }
        free(board.tile);
    }

    board.tile = calloc(board.len.x, sizeof(Tile*));
    for(int x = 0; x < board.len.x; x++)
        board.tile[x] = calloc(board.len.y, sizeof(Tile));

    return board;
}

bool iLen(const int x, const int y, const Coord len)
{
    return x > 0 && y > 0 && x < len.x && y < len.y;
}

bool cLen(const Coord pos, const Coord len)
{
    return iLen(pos.x, pos.y, len);
}

Coord cOff(const Coord pos, const Coord off)
{
    return (const Coord){.x=pos.x+off.x, .y=pos.y+off.y};
}

bool cEq(const Coord a, const Coord b)
{
    return a.x == b.x && a.y == b.y;
}

Board floodAt(Board board, const Coord pos)
{
    board.tile[pos.x][pos.y].state = TS_REVL;
    const int num = board.tile[pos.x][pos.y].num;
    if(num == -1)
        board.state = G_LOST;
    if(num == -1 || num != 0)
        return board;

    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            const Coord adjpos = (const Coord){.x = pos.x+x, .y = pos.y+y};
            if((x == 0 && y == 0) || !cLen(adjpos, board.len))
                continue;
            const int adjnum = board.tile[adjpos.x][adjpos.y].num;
            const TileState adjstate = board.tile[adjpos.x][adjpos.y].state;
            if(adjnum == 0 && adjstate == TS_HIDE)
                board = floodAt(board, adjpos);
            else if(adjnum > 0  && adjstate == TS_HIDE)
                board.tile[adjpos.x][adjpos.y].state = TS_REVL;
        }
    }
    return board;
}

uint boardTilesLeft(const Board board)
{
    uint left = 0;
    for(int y = 0; y < board.len.y; y++)
        for(int x = 0; x < board.len.x; x++)
            left += board.tile[x][y].num >= 0 && board.tile[x][y].state != TS_REVL;

    return left;
}

Board boardIncAdj(Board board, const Coord pos)
{
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            const Coord adjpos = {.x = pos.x + x, .y = pos.y + y};
            if((x == 0 && y == 0) || !cLen(adjpos, board.len) || board.tile[adjpos.x][adjpos.y].num == -1)
                continue;
            board.tile[adjpos.x][adjpos.y].num++;
        }
    }
    return board;
}

Board boardSetNums(Board board)
{
    for(int y = 0; y < board.len.y; y++)
        for(int x = 0; x < board.len.x; x++)
            if(board.tile[x][y].num == -1)
                board = boardIncAdj(board, (const Coord){.x=x, .y=y});

    return board;
}

Board boardPlaceBombs(Board board, const Coord fst)
{
    for(uint i = 0; i < board.numBombs; i++){
        Coord pos = {0};
        do{
            pos.x = rand() % board.len.x;
            pos.y = rand() % board.len.y;
        }while(
            cEq(pos, fst) ||
            board.tile[pos.x][pos.y].num == -1 || (
                pos.x >= fst.x-1 && pos.x <= fst.x+1 &&
                pos.y >= fst.y-1 && pos.y <= fst.y+1
            )
        );
        board.tile[pos.x][pos.y].num = -1;
    }
    board.state = G_PLAY;
    return floodAt(boardSetNums(board), fst);
}

void actionHelp(void)
{
    fputs("Action help -\n", stdout);
    fputs("\tTo reveal a tile:\n\t\tr <x position> <y positon>\n", stdout);
    fputs("\tTo flag a tile:\n\t\tf <x position> <y positon>\n", stdout);
    fputs("\tTo question mark a tile:\n\t\tq <x position> <y positon>\n", stdout);
    fputs("\tTo unmark a tile:\n\t\tu <x position> <y positon>\n", stdout);

}

void flush(void)
{
    int c = ' ';
    while((c = fgetc(stdin)) != '\n' && c != '\0');
}

uint strnlen(const char *str, const uint max)
{
    for(uint i = 0; i < max; i++)
        if(str[i] == '\0' || str[i] =='\n')
            return i;
    return max;
}

TileState actionChar(const char c)
{
    switch(c){
        case 't':
            return TS_HIDE;
            break;
        case 'r':
            return TS_REVL;
            break;
        case 'f':
            return TS_FLAG;
            break;
        case 'q':
            return TS_QEST;
            break;
        default:
            break;
    }
    return TS_INVL;
}

bool parseNum(char *str, int *num, const int max)
{
    char *end = NULL;
    unsigned long u = strtoul(str, &end, 0);
    if(errno == ERANGE){
        errno = 0;
        return false;
    }
    if((int)u > max)
        return false;
    *num = (int)u;
    return true;
}

bool actionValidate(const Board board, char *str, Action *act)
{
    if(NULL == str)
        return false;

    const uint slen = strnlen(str, 64);
    if(slen == 63 && str[62] != '\0'){
        flush();
        return false;
    }

    if(slen < 5 || slen > 7){
        if(str[slen] != '\0' && str[slen] != '\n')
        printf("1 %u\n", slen);
        return false;
    }

    if((act->type = actionChar(str[0])) == TS_INVL || !isspace(str[1]) || !isdigit(str[2])){
        printf("2\n");
        return false;
    }

    if(!parseNum(&str[2], &(act->pos.x), board.len.x)){
        printf("3\n");
        return false;
    }

    if(isdigit(str[3])){
        str = &str[5];
    }else{
        if(!isspace(str[3])){
            printf("4\nstr: \"%s\"\n", str);
            return false;
        }
        str = &str[4];
    }

    if(!isdigit(*str)){
        printf("5\nstr: \"%s\"\n", str);
        return false;
    }

    if(!parseNum(str, &(act->pos.y), board.len.y)){
        printf("6\nstr: \"%s\"\n", str);
        return false;
    }

    if(str[1] != '\n' && str[2] != '\n'){
        printf("7\nstr: \"%s\"\n", str);
        return false;
    }

    return true;
}

void actionPrint(const Action act)
{
    fputs("Action -\n", stdout);
    printf("\tact.type: %s\n", TileStateStr[act.type]);
    printf("\tact.pos: (%u,%u)\n\n", act.pos.x, act.pos.y);
}

Action actionParse(const Board board)
{
    Action act = {0};
    while(1){
        char buf[64] = {0};
        buf[0] = '\0';
        fputs("Enter an action or ? for help.\n", stdout);
        if(!fgets(buf, 64, stdin) || strnlen(buf, 16) < 5 || buf[0] == '?' || !actionValidate(board, buf, &act))
            actionHelp();
        else
            break;
    }
    actionPrint(act);
    return act;
}

int main(int argc, char** argv)
{
    Board board = {0};
    if(argc == 1){
        board.len.x = 30;
        board.len.y = 16;
        board.numBombs = 99;
    }else if(argc == 4){
        board = boardParse(&argv[1]);
    }else{
        usage();
        exit(EXIT_FAILURE);
    }
    board = boardAlloc(board);
    srand(time(0));
    boardPrintInfo(board);
    boardPrint(board, false);
    Action act = {0};
    do{
        act = actionParse(board);
    }while(act.type != TS_REVL);
    board = boardPlaceBombs(board, act.pos);
    while(board.state == G_PLAY){
        boardPrint(board, false);
        act = actionParse(board);
        switch(act.type){
            case TS_HIDE:
                if(board.tile[act.pos.x][act.pos.y].state != TS_REVL)
                    board.tile[act.pos.x][act.pos.y].state = TS_HIDE;
                break;
            case TS_REVL:
                board = floodAt(board, act.pos);
                if(board.state != G_LOST && boardTilesLeft(board) == 0)
                    board.state = G_WIN;
                break;
            case TS_FLAG:
                if(board.tile[act.pos.x][act.pos.y].state != TS_REVL)
                    board.tile[act.pos.x][act.pos.y].state = TS_FLAG;
                break;
            case TS_QEST:
                if(board.tile[act.pos.x][act.pos.y].state != TS_REVL)
                    board.tile[act.pos.x][act.pos.y].state = TS_QEST;
                break;
            default:
                fprintf(stderr, "act.type is invalid(%i)\n", act.type);
                exit(EXIT_FAILURE);
        }
    }
    boardPrint(board, true);
    board = boardFree(board);

    return 0;
}
