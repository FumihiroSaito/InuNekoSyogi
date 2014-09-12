#include <stdio.h>
#include <stdlib.h>

#define MASUME_NUM 10
#define KOMA_NUM 40
#define KOMADAI_NUM 2
#define TEMOTI_MAX_NUM 40

typedef struct __KOMA{
        int id;
        int koma_num; //駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8
        int yoko; //0駒台
        int tate; //0駒台
        int sente_gote; //0先手1後手
        int nari_funari; //0駒台1成2不成
        int kachi; //コマの価値
}KOMA;

typedef struct __BAN{
        int masu[MASUME_NUM][MASUME_NUM];
}BAN;

typedef struct __KOMADAI{
        int komadai[KOMADAI_NUM][TEMOTI_MAX_NUM];
}KOMADAI;

typedef struct __SENKYOU{
        int sente_senkyou;
        int gote_senkyou;
}SENKYOKU;

typedef struct __KYOKUMEN{
        int sente_gote;
        KOMA* koma[KOMA_NUM];
        BAN ban;
        KOMADAI komadai;
        SENKYOKU senkyoku;
}KYOKUMEN;


/* 指定されたコマがそのマス目にあるかをチェックする */
int Sitei_Koma_check(KYOKUMEN* kyoku,int yoko,int tate,int koma_num){
        int id;

        id = kyoku->ban.masu[yoko][tate];
        if(id == 0){
                fprintf(stdout,"そこにはコマは存在しません\n");
                return -1;
        }
        if((kyoku->koma[id]->koma_num == koma_num) && (kyoku->koma[id]->sente_gote == kyoku->sente_gote)){
        /* 指定されたコマと置いてあるコマの駒番号は一致し、かつ自分のコマである */
                return 0;
        }else if(kyoku->koma[id]->koma_num != koma_num){
                fprintf(stdout,"指定されたコマはそこにはありません。そこには別のコマがあります。\n");
                return -1;
        }else if(kyoku->koma[id]->sente_gote != kyoku->sente_gote){
                fprintf(stdout,"指定されたコマは相手のコマです\n");
                return -1;
        }
}

/* 指定された場所に駒があるかを判定する。敵の駒か見方の駒かも判定する。 */
/* 戻り値0:コマがない -1:敵のコマがある -2:味方のコマがある*/
int Koma_Sonzai_Check(KYOKUMEN* kyoku,int yoko,int tate){

        int ret=0;
        /* 指定されたマスに駒があれば、戻り値を-1 */
        if(kyoku->ban.masu[yoko][tate] != 0){
                /* 戻り値-2:その駒は味方のものである */
                ret = ret + Koma_Teki_Mikata_check(kyoku,yoko,tate);
                return ret;
        }else if(kyoku->ban.masu[yoko][tate] == 0){
                /* コマがそこにはない */
                return 0;
        }
}

/* 指定された場所の駒が敵か味方かを判定する */
int Koma_Teki_Mikata_check(KYOKUMEN* kyoku,int yoko,int tate){
        int id;

        id = kyoku->ban.masu[yoko][tate];
        if(id == 0){
                fprintf(stdout,"指定された場所にコマはありません\n");
                return 0;
        }
        if(kyoku->koma[id]->sente_gote != kyoku->sente_gote){
        /* 敵のコマ */
                return -1;
        }else{
        /* 味方のコマ */
                return -2;
        }
}

/* 指定された場所のコマを取る */
KYOKUMEN* Toru(KYOKUMEN* kyoku,int yoko,int tate){
        int id;
        /* ますにあるコマを特定する */
        id = kyoku->ban.masu[yoko][tate];

        kyoku->ban.masu[yoko][tate] = 0;
        kyoku->koma[id]->yoko = 0;
        kyoku->koma[id]->tate = 0;
        kyoku->koma[id]->nari_funari = 0;
        if(kyoku->koma[id]->sente_gote == 0){
                kyoku->komadai.komadai[1][id] = id;
                kyoku->koma[id]->sente_gote = 1;
        }else{
                kyoku->komadai.komadai[0][id] = id;
                kyoku->koma[id]->sente_gote = 0;
        }

        return kyoku;
}

/* 盤面を表示する */
void BanHyouji(KYOKUMEN* kyoku){
        if(kyoku->sente_gote == 0){
                fprintf(stdout,"先手番です\n");
        }else if(kyoku->sente_gote == 1){
                fprintf(stdout,"後手番です\n");
        }
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][1]
                                        ,kyoku->ban.masu[8][1]
                                        ,kyoku->ban.masu[7][1]
                                        ,kyoku->ban.masu[6][1]
                                        ,kyoku->ban.masu[5][1]
                                        ,kyoku->ban.masu[4][1]
                                        ,kyoku->ban.masu[3][1]
                                        ,kyoku->ban.masu[2][1]
                                        ,kyoku->ban.masu[1][1]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][2]
                                        ,kyoku->ban.masu[8][2]
                                        ,kyoku->ban.masu[7][2]
                                        ,kyoku->ban.masu[6][2]
                                        ,kyoku->ban.masu[5][2]
                                        ,kyoku->ban.masu[4][2]
                                        ,kyoku->ban.masu[3][2]
                                        ,kyoku->ban.masu[2][2]
                                        ,kyoku->ban.masu[1][2]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][3]
                                        ,kyoku->ban.masu[8][3]
                                        ,kyoku->ban.masu[7][3]
                                        ,kyoku->ban.masu[6][3]
                                        ,kyoku->ban.masu[5][3]
                                        ,kyoku->ban.masu[4][3]
                                        ,kyoku->ban.masu[3][3]
                                        ,kyoku->ban.masu[2][3]
                                        ,kyoku->ban.masu[1][3]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][4]
                                        ,kyoku->ban.masu[8][4]
                                        ,kyoku->ban.masu[7][4]
                                        ,kyoku->ban.masu[6][4]
                                        ,kyoku->ban.masu[5][4]
                                        ,kyoku->ban.masu[4][4]
                                        ,kyoku->ban.masu[3][4]
                                        ,kyoku->ban.masu[2][4]
                                        ,kyoku->ban.masu[1][4]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][5]
                                        ,kyoku->ban.masu[8][5]
                                        ,kyoku->ban.masu[7][5]
                                        ,kyoku->ban.masu[6][5]
                                        ,kyoku->ban.masu[5][5]
                                        ,kyoku->ban.masu[4][5]
                                        ,kyoku->ban.masu[3][5]
                                        ,kyoku->ban.masu[2][5]
                                        ,kyoku->ban.masu[1][5]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][6]
                                        ,kyoku->ban.masu[8][6]
                                        ,kyoku->ban.masu[7][6]
                                        ,kyoku->ban.masu[6][6]
                                        ,kyoku->ban.masu[5][6]
                                        ,kyoku->ban.masu[4][6]
                                        ,kyoku->ban.masu[3][6]
                                        ,kyoku->ban.masu[2][6]
                                        ,kyoku->ban.masu[1][6]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][7]
                                        ,kyoku->ban.masu[8][7]
                                        ,kyoku->ban.masu[7][7]
                                        ,kyoku->ban.masu[6][7]
                                        ,kyoku->ban.masu[5][7]
                                        ,kyoku->ban.masu[4][7]
                                        ,kyoku->ban.masu[3][7]
                                        ,kyoku->ban.masu[2][7]
                                        ,kyoku->ban.masu[1][7]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][8]
                                        ,kyoku->ban.masu[8][8]
                                        ,kyoku->ban.masu[7][8]
                                        ,kyoku->ban.masu[6][8]
                                        ,kyoku->ban.masu[5][8]
                                        ,kyoku->ban.masu[4][8]
                                        ,kyoku->ban.masu[3][8]
                                        ,kyoku->ban.masu[2][8]
                                        ,kyoku->ban.masu[1][8]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
        fprintf(stdout,"| %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d | %2d |\n"
                                        ,kyoku->ban.masu[9][9]
                                        ,kyoku->ban.masu[8][9]
                                        ,kyoku->ban.masu[7][9]
                                        ,kyoku->ban.masu[6][9]
                                        ,kyoku->ban.masu[5][9]
                                        ,kyoku->ban.masu[4][9]
                                        ,kyoku->ban.masu[3][9]
                                        ,kyoku->ban.masu[2][9]
                                        ,kyoku->ban.masu[1][9]
                                        );
        fprintf(stdout," ---- ---- ---- ---- ---- ---- ---- ---- ----\n");

        int i;
        /* 手持ちのコマを表示する */
        for(i=1;i<=KOMA_NUM;i++){
                if(kyoku->koma[i]->nari_funari == 0){
                        if(kyoku->koma[i]->sente_gote == 0){
                                fprintf(stdout,"先手駒台:%d\n",kyoku->koma[i]->id);
                        }else if(kyoku->koma[i]->sente_gote == 1){
                                fprintf(stdout,"           後手駒台:%d\n",kyoku->koma[i]->id);
                        }
                }
        }
}

/* 動く範囲を判定する */
int Ugoku_Hanni_Hantei(KYOKUMEN* kyoku,int yoko_before,int tate_before,int yoko_after,int tate_after,int id){

        int i,j;

        /* 先手 */
        if(kyoku->sente_gote == 0){
                /* 玉 */
                if(kyoku->koma[id]->koma_num == 1){
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 後ろ */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before - 1 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before == tate_after)){
                        /* 左 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before == tate_after)){
                        /* 右 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 左斜め後ろ */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 右斜め後ろ */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"玉はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 2){
                /* 飛車 */
                        if((yoko_before == yoko_after) || (tate_before == tate_after)){
                                if(yoko_before == yoko_after){
                                /* 前後に進む */
                                        if(tate_before > tate_after){
                                        /* 先手側から見て前にすすむ */
                                                for(i = tate_before - 1;i > tate_after;i--){
                                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }else if(tate_before < tate_after){
                                        /* 先手側から見て後ろに進む */
                                                for(i = tate_before + 1;i < tate_after;i++){
                                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }
                                }else if(tate_before == tate_after){
                                /* 左右に進む */
                                        if(tate_before > tate_after){
                                        /* 先手側から見て右にすすむ */
                                                for(i = yoko_before - 1;i > yoko_after;i--){
                                                        if(kyoku->ban.masu[i][tate_before] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",i,tate_before);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }else if(tate_before < tate_after){
                                        /* 先手側から見て左に進む */
                                                for(i = yoko_before + 1;i < yoko_after;i++){
                                                        if(kyoku->ban.masu[i][tate_before] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",i,tate_before);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"飛車はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 3){
                /* 角 */
                        if(((yoko_after - yoko_before) == (tate_after - tate_before)) && (yoko_after < yoko_before) && (tate_after < tate_before)){
                        /* 左斜め前 */
                                for(i = yoko_before -1; i > yoko_after ;i--){
                                        for(j = tate_before -1; j > tate_after;j--){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == (tate_after - tate_before)) && (yoko_after > yoko_before) && (tate_after > tate_before)){
                        /* 左斜め後ろ */
                                for(i = yoko_before + 1; i < yoko_after ;i++){
                                        for(j = tate_before + 1; j < tate_after;j++){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == -(tate_after - tate_before)) && (yoko_after > yoko_before) && (tate_after < tate_before)){
                        /* 左斜め前 */
                                for(i = yoko_before +1; i < yoko_after ;i++){
                                        for(j = tate_before -1; j > tate_after;j--){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == -(tate_after - tate_before)) && (yoko_after < yoko_before) && (tate_after > tate_before)){
                        /* 右斜め後ろ */
                                for(i = yoko_before - 1; i > yoko_after ;i--){
                                        for(j = tate_before + 1; j < tate_after;j++){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 4){
                /* 金 */
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 後ろ */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before - 1 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before == tate_after)){
                        /* 左 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before == tate_after)){
                        /* 右 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"金はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 5){
                /* 銀 */
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before - 1 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 左斜め後ろ */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 右斜め後ろ */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"銀はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 6){
                /* 桂 */
                        if((yoko_before + 1 == yoko_after) && (tate_before - 2 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 2 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"桂はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 7){
                /* 香 */
                        if((yoko_before == yoko_after) && (tate_before > tate_after)){
                                for(i = tate_before - 1;i > tate_after;i--){
                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                        return -1;
                                        }else{
                                                        return 0;
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"香はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 8){
                /* 歩 */
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"歩はその範囲には動けません\n");
                                return -1;
                        }
                }
        }else if(kyoku->sente_gote == 1){
                /* 玉 */
                if(kyoku->koma[id]->koma_num == 1){
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 後ろ */
                                return 0;
                        }else if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before - 1 ==  tate_after)){
                        /* 右斜め後ろ */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 左斜め後ろ */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before == tate_after)){
                        /* 右 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before == tate_after)){
                        /* 左 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"玉はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 2){
                /* 飛車 */
                        if((yoko_before == yoko_after) || (tate_before == tate_after)){
                                if(yoko_before == yoko_after){
                                /* 前後に進む */
                                        if(tate_before > tate_after){
                                        /* 先手側から見て前にすすむ */
                                                for(i = tate_before - 1;i > tate_after;i--){
                                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }else if(tate_before < tate_after){
                                        /* 先手側から見て後ろに進む */
                                                for(i = tate_before + 1;i < tate_after;i++){
                                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }
                                }else if(tate_before == tate_after){
                                /* 左右に進む */
                                        if(tate_before > tate_after){
                                        /* 先手側から見て右にすすむ */
                                                for(i = yoko_before - 1;i > yoko_after;i--){
                                                        if(kyoku->ban.masu[i][tate_before] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",i,tate_before);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }else if(tate_before < tate_after){
                                        /* 先手側から見て左に進む */
                                                for(i = yoko_before + 1;i < yoko_after;i++){
                                                        if(kyoku->ban.masu[i][tate_before] != 0){
                                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",i,tate_before);
                                                                return -1;
                                                        }else{
                                                                return 0;
                                                        }
                                                }
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"飛車はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 3){
                /* 角 */
                        if(((yoko_after - yoko_before) == (tate_after - tate_before)) && (yoko_after < yoko_before) && (tate_after < tate_before)){
                        /* 先手側から見て左斜め前 */
                                for(i = yoko_before - 1; i > yoko_after ;i--){
                                        for(j = tate_before - 1; j > tate_after;j--){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == (tate_after - tate_before)) && (yoko_after > yoko_before) && (tate_after > tate_before)){
                        /* 先手側から見て左斜め後ろ */
                                for(i = yoko_before + 1; i < yoko_after ;i++){
                                        for(j = tate_before + 1; j < tate_after;j++){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == -(tate_after - tate_before)) && (yoko_after > yoko_before) && (tate_after < tate_before)){
                        /* 先手側から見て左斜め前 */
                                for(i = yoko_before + 1; i < yoko_after ;i++){
                                        for(j = tate_before; j > tate_after;j--){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else if(((yoko_after - yoko_before) == -(tate_after - tate_before)) && (yoko_after < yoko_before) && (tate_after > tate_before)){
                        /* 先手側から見て右斜め後ろ */
                                for(i = yoko_before - 1; i > yoko_after ;i--){
                                        for(j = tate_before; j < tate_after;j++){
                                                if(kyoku->ban.masu[i][j] !=0){
                                                        fprintf(stdout,"%d %d の位置にコマがあるので動けません\n",i,j);
                                                        return -1;
                                                }else{
                                                        return 0;
                                                }
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 4){
                /* 金 */
                        if((yoko_before == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 後ろ */
                                return 0;
                        }else if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before + 1 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before == tate_after)){
                        /* 左 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before == tate_after)){
                        /* 右 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"金はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 5){
                /* 銀 */
                        if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before + 1 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else if((yoko_before - 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 左斜め後ろ */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before - 1 == tate_after)){
                        /* 右斜め後ろ */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"銀はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 6){
                /* 桂 */
                        if((yoko_before - 1 == yoko_after) && (tate_before + 2 ==  tate_after)){
                        /* 左斜め前 */
                                return 0;
                        }else if((yoko_before + 1 == yoko_after) && (tate_before + 2 == tate_after)){
                        /* 右斜め前 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"桂はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 7){
                /* 香 */
                        if((yoko_before == yoko_after) && (tate_before > tate_after)){
                                for(i = tate_before + 1;i < tate_after;i++){
                                        if(kyoku->ban.masu[yoko_before][i] != 0){
                                                fprintf(stdout,"%d %dの位置にコマがあるので動けません\n",yoko_before,i);
                                                        return -1;
                                        }else{
                                                        return 0;
                                        }
                                }
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"香はその範囲には動けません\n");
                                return -1;
                        }
                }else if(kyoku->koma[id]->koma_num == 8){
                /* 歩 */
                        if((yoko_before == yoko_after) && (tate_before + 1 == tate_after)){
                        /* 前 */
                                return 0;
                        }else{
                        /* 範囲外 */
                                fprintf(stdout,"歩はその範囲には動けません\n");
                                return -1;
                        }
                }
        }
}

/* コマを打つ */
int Utu(KYOKUMEN* kyoku){
        int i,id,yoko,tate,ret;

        fprintf(stdout,"打つコマを指定してください\n");
        if(!scanf("%d",&id)){
                fprintf(stdout,"入力値に異常があります");
                return -1;
        }
        if(kyoku->komadai.komadai[kyoku->sente_gote][id] == 0){
                fprintf(stdout,"その駒は駒台にはありません\n");
                return -1;
        }
        fprintf(stdout,"どこに置きますか?\n");
        fprintf(stdout,"横:");
        if(!scanf("%d",&yoko)){
                fprintf(stdout,"入力値に異常があります");
                return -1;
        }
        if((yoko < 1) || (10 < yoko)){
                fprintf(stdout,"そこにコマは置けません\n");
                return -1;
        }
        fprintf(stdout,"縦:");
        if(!scanf("%d",&tate)){
                fprintf(stdout,"入力値に異常があります\n");
                return -1;
        }
        if((tate < 1) || (10 <tate)){
                fprintf(stdout,"そこにコマは置けません\n");
                return -1;
        }
        ret = Koma_Sonzai_Check(kyoku,yoko,tate);
        if(ret < 0){
                fprintf(stdout,"そこにコマが既にあります\n");
                return -1;
        }else if(ret == 0){
                kyoku->ban.masu[yoko][tate] = id;
                kyoku->koma[id]->yoko = yoko;
                kyoku->koma[id]->tate = tate;
                kyoku->koma[id]->nari_funari = 2;
                return 0;
        }
}

/* 動かす */
KYOKUMEN* Ugokasu(KYOKUMEN* kyoku){

        int id;
        int yoko_before;
        int tate_before;
        int yoko_after;
        int tate_after;
        int koma_num;
        int err = -1;

        while(1){
                /* 何を */
                fprintf(stdout,"動かす駒を指定してください\n");
                fprintf(stdout,"駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8\n");
                /* 横の読み込み */
                fprintf(stdout,"横:");
                if(!scanf("%d",&yoko_before)){
                        fprintf(stdout,"入力値に異常があります\n");;
                        continue;
                }
                if((yoko_before < 0) || (10 < yoko_before)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                /* 縦の読み込み */
                fprintf(stdout,"縦:");
                if(!scanf("%d",&tate_before)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                if((tate_before < 0) || (10 < tate_before)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                /* 0 0が指定されたらコマを打つ */
                if((yoko_before == 0) && (tate_before == 0)){
                        if( Utu(kyoku) == -1){
                                continue;
                        }else{
                                goto Utta;
                        }
                }
                /* 駒番号読み込み */
                fprintf(stdout,"駒番号:");
                if(!scanf("%d",&koma_num)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                if((koma_num < 1) || (8 < koma_num)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                /* 指定された駒がそこにあるかを判定する */
                err = Sitei_Koma_check(kyoku,yoko_before,tate_before,koma_num);
                if(err != 0){
                        continue;
                }
                break;
        }

        /* 動かすコマのIDを特定する */
        id = kyoku->ban.masu[yoko_before][tate_before];

        while(1){
                /* どこに */
                fprintf(stdout,"動かす場所を指定してください");
                fprintf(stdout,"駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8\n");
                /* 横の読み込み */
                fprintf(stdout,"横:");
                if(!scanf("%d",&yoko_after)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                if((yoko_after < 1) || (10 < yoko_after)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                /* 縦の読み込み */
                fprintf(stdout,"縦:");
                if(!scanf("%d",&tate_after)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                if((tate_after < 1) || (10 < tate_after)){
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }

                /* 指定された駒が、指定した場所に動けるか判定する */
                err = Ugoku_Hanni_Hantei(kyoku,yoko_before,tate_before,yoko_after,tate_after,id);
                if(err == -1){
                /* 範囲が間違っていてコマを動かせない */
                        continue;
                }else{
                /* 範囲はあっている */
                        /* 駒が動かす先にあるかを判定する（同時にその駒の敵味方も判定する) */
                        err = Koma_Sonzai_Check(kyoku,yoko_after,tate_after);
                        /*
                        * 戻り値0:そこに駒はない。
                        * 戻り値-1:その駒は敵のコマである。
                        * 戻り値-2:その駒は味方のコマである。
                         */
                        if(err != 0){
                                if(err == -1){
                                /* 敵のコマなので取る!!!!!! */
                                        kyoku = Toru(kyoku,yoko_after,tate_after);
                                        break;
                                }
                                else if(err == -2){
                                        fprintf(stdout,"取ろうとしている駒は味方のものです\n");
                                        continue;
                                }
                        }else if(err == 0){
                        /* 動かせる！！！！！！ */
                                break;
                        }
                }
        }

        /* 指定した場所にコマを動かす */
        kyoku->ban.masu[yoko_after][tate_after] = id;
        kyoku->ban.masu[yoko_before][tate_before] = 0;
        kyoku->koma[id]->yoko = yoko_after;
        kyoku->koma[id]->tate = tate_after;
        /* なるかどうかを決める*/
        int tmp;
        if((kyoku->koma[id]->sente_gote == 0) && (tate_after <= 3) && (kyoku->koma[id]->nari_funari == 2)){
                while(1){
                        fprintf(stdout,"成りますか？成:1 不成:2\n");
                        if(!scanf("%d",&tmp)){
                                fprintf(stdout,"入力値に異常があります\n");
                        }else{
                                if(tmp == 1){
                                        kyoku->koma[id]->nari_funari = 1;
                                        break;
                                }else if(tmp == 2){
                                        kyoku->koma[id]->nari_funari = 2;
                                        break;
                                }else{
                                        fprintf(stdout,"入力値に異常があります\n");
                                        continue;
                                }
                        }
                }
        }

        /* 盤表示をする*/
        BanHyouji(kyoku);
        return kyoku;

        /* 打った */
        Utta:
        BanHyouji(kyoku);
        return kyoku;
}

/* ゲームの実行 */
void play(KYOKUMEN* kyoku){
        int play = 0;
        kyoku->sente_gote = 0;//0:先手 1:後手

        while(play != 1){
                fprintf(stdout,"続行:0 終了:1\n");
                if(!scanf("%d",&play)){;
                        fprintf(stdout,"入力値に異常があります\n");
                        continue;
                }
                if(play == 1){
                /* 終了 */
                        break;
                }else if(play == 0){
                /* 一手分プレイする */
                        BanHyouji(kyoku);
                        kyoku = Ugokasu(kyoku);
                        /* 先手後手を入れ替える */
                        if(kyoku->sente_gote == 0){
                                kyoku->sente_gote = 1;
                        }else if(kyoku->sente_gote == 1){
                                kyoku->sente_gote = 0;
                        }
                }else{
                        continue;
                }
        }
}

/* 駒を作る */
KOMA* make_koma(int id,int koma_num,int yoko,int tate,int sente_gote,int nari_funari,int kachi){

        KOMA* koma;
        koma = (KOMA *)malloc(sizeof(KOMA));
        if(koma == NULL){
                fprintf(stdout,"KOMAを確保できませんでした\n");
                exit(1);
        }

        koma->id = id;
        koma->koma_num = koma_num; /* 駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8 */
        koma->yoko = yoko;
        koma->tate = tate;
        koma->sente_gote = sente_gote; /* 0先手1後手 */
        koma->nari_funari = nari_funari; /* 0駒台1成2不成 */
        koma->kachi = kachi;

        return koma;
}

/* 盤を作る */
void make_ban(KYOKUMEN* kyoku){

        int i,j;
        /* masu[i][j]には、そこに乗っている駒のidが入る */
        for(i=0;i<MASUME_NUM;i++){
                for(j=0;j<MASUME_NUM;j++){
                        kyoku->ban.masu[i][j] = 0;
                }
        }
}

/* 駒,盤を初期化する */
void init_koma_ban(KYOKUMEN* kyoku){

        int i;

        for(i=1;i<=KOMA_NUM;i++){
                if(i==1){
                /* 先手玉 */
                        kyoku->koma[i] = make_koma(1,1,5,9,0,2,200);
                        kyoku->ban.masu[5][9] = 1;
                }else if(i==2){
                /* 後手玉 */
                        kyoku->koma[i] = make_koma(2,1,5,1,1,2,200);
                        kyoku->ban.masu[5][1] = 2;
                }else if(i==3){
                        /* 先手飛 */
                        kyoku->koma[i] = make_koma(3,2,2,8,0,2,50);
                        kyoku->ban.masu[2][8] = 3;
                }else if(i==4){
                        /* 後手飛 */
                        kyoku->koma[i] = make_koma(4,2,8,2,1,2,50);
                        kyoku->ban.masu[8][2] = 4;
                }else if(i==5){
                        /* 先手角 */
                        kyoku->koma[i] = make_koma(5,3,8,8,0,2,50);
                        kyoku->ban.masu[8][8] = 5;
                }else if(i==6){
                        /* 後手角 */
                        kyoku->koma[i] = make_koma(6,3,2,2,1,2,50);
                        kyoku->ban.masu[2][2] = 6;
                }else if(i==7){
                        /* 先手金1 */
                        kyoku->koma[i] = make_koma(7,4,4,9,0,2,40);
                        kyoku->ban.masu[4][9] = 7;
                }else if(i==8){
                        /* 先手金2 */
                        kyoku->koma[i] = make_koma(8,4,6,9,1,2,40);
                        kyoku->ban.masu[6][9] = 8;
                }else if(i==9){
                        /* 後手金3 */
                        kyoku->koma[i] = make_koma(9,4,4,1,1,2,40);
                        kyoku->ban.masu[4][1] = 9;
                }else if(i==10){
                        /* 後手金4 */
                        kyoku->koma[i] = make_koma(10,4,6,1,1,2,40);
                        kyoku->ban.masu[6][1] = 10;
                }else if(i==11){
                        /* 先手銀1 */
                        kyoku->koma[i] = make_koma(11,5,3,9,0,2,30);
                        kyoku->ban.masu[3][9] = 11;
                }else if(i==12){
                        /* 先手銀2 */
                        kyoku->koma[i] = make_koma(12,5,7,9,0,2,30);
                        kyoku->ban.masu[7][9] = 12;
                }else if(i==13){
                        /* 後手銀3 */
                        kyoku->koma[i] = make_koma(13,5,3,1,1,2,30);
                        kyoku->ban.masu[3][1] = 13;
                }else if(i==14){
                        /* 後手銀4 */
                        kyoku->koma[i] = make_koma(14,5,7,1,1,2,30);
                        kyoku->ban.masu[7][1] = 14;
                }else if(i==15){
                        /* 先手桂1 */
                        kyoku->koma[i] = make_koma(15,6,2,9,0,2,20);
                        kyoku->ban.masu[2][9] = 15;
                }else if(i==16){
                        /* 先手桂2 */
                        kyoku->koma[i] = make_koma(16,6,8,9,0,2,20);
                        kyoku->ban.masu[8][9] = 16;
                }else if(i==17){
                        /* 後手桂3 */
                        kyoku->koma[i] = make_koma(17,6,2,1,1,2,20);
                        kyoku->ban.masu[2][1] = 17;
                }else if(i==18){
                        /* 後手桂4 */
                        kyoku->koma[i] = make_koma(18,6,8,1,1,2,20);
                        kyoku->ban.masu[8][1] = 18;
                }else if(i==19){
                        /* 先手香1 */
                        kyoku->koma[i] = make_koma(19,7,1,9,0,2,10);
                        kyoku->ban.masu[1][9] = 19;
                }else if(i==20){
                        /* 先手香2 */
                        kyoku->koma[i] = make_koma(20,7,9,9,0,2,10);
                        kyoku->ban.masu[9][9] = 20;
                }else if(i==21){
                        /* 後手香3 */
                        kyoku->koma[i] = make_koma(21,7,1,1,1,2,10);
                        kyoku->ban.masu[1][1] = 21;
                }else if(i==22){
                        /* 後手香4 */
                        kyoku->koma[i] = make_koma(22,7,9,1,1,2,10);
                        kyoku->ban.masu[9][1] = 22;
                }else if(i==23){
                        /* 先手歩1 */
                        kyoku->koma[i] = make_koma(23,8,1,7,0,2,1);
                        kyoku->ban.masu[1][7] = 23;
                }else if(i==24){
                        /* 先手歩2 */
                        kyoku->koma[i] = make_koma(24,8,2,7,0,2,1);
                        kyoku->ban.masu[2][7] = 24;
                }else if(i==25){
                /* 先手歩3 */
                        kyoku->koma[i] = make_koma(25,8,3,7,0,2,1);
                        kyoku->ban.masu[3][7] = 25;
                }else if(i==26){
                /* 先手歩4 */
                        kyoku->koma[i] = make_koma(26,8,4,7,0,2,1);
                        kyoku->ban.masu[4][7] = 26;
                }else if(i==27){
                /* 先手歩5 */
                        kyoku->koma[i] = make_koma(27,8,5,7,0,2,1);
                        kyoku->ban.masu[5][7] = 27;
                }else if(i==28){
                /* 先手歩6 */
                        kyoku->koma[i] = make_koma(28,8,6,7,0,2,1);
                        kyoku->ban.masu[6][7] = 28;
                }else if(i==29){
                /* 先手歩7 */
                        kyoku->koma[i] = make_koma(29,8,7,7,0,2,1);
                        kyoku->ban.masu[7][7] = 29;
                }else if(i==30){
                /* 先手歩8 */
                        kyoku->koma[i] = make_koma(30,8,8,7,0,2,1);
                        kyoku->ban.masu[8][7] = 30;
                }else if(i==31){
                /* 先手歩9 */
                        kyoku->koma[i] = make_koma(31,8,9,7,0,2,1);
                        kyoku->ban.masu[9][7] = 31;
                }else if(i==32){
                /* 後手歩1 */
                        kyoku->koma[i] = make_koma(32,8,1,3,1,2,1);
                        kyoku->ban.masu[1][3] = 32;
                }else if(i==33){
                /* 後手歩2 */
                        kyoku->koma[i] = make_koma(33,8,2,3,1,2,1);
                        kyoku->ban.masu[2][3] = 33;
                }else if(i==34){
                /* 後手歩3 */
                        kyoku->koma[i] = make_koma(34,8,3,3,1,2,1);
                        kyoku->ban.masu[3][3] = 34;
                }else if(i==35){
                /* 後手歩4 */
                        kyoku->koma[i] = make_koma(35,8,4,3,1,2,1);
                        kyoku->ban.masu[4][3] = 35;
                }else if(i==36){
                /* 後手歩5 */
                        kyoku->koma[i] = make_koma(36,8,5,3,1,2,1);
                        kyoku->ban.masu[5][3] = 36;
                }else if(i==37){
                /* 後手歩6 */
                        kyoku->koma[i] = make_koma(37,8,6,3,1,2,1);
                        kyoku->ban.masu[6][3] = 37;
                }else if(i==38){
                /* 後手歩7 */
                        kyoku->koma[i] = make_koma(38,8,7,3,1,2,1);
                        kyoku->ban.masu[7][3] = 38;
                }else if(i==39){
                /* 後手歩8 */
                        kyoku->koma[i] = make_koma(39,8,8,3,1,2,1);
                        kyoku->ban.masu[8][3] = 39;
                }else if(i==40){
                /* 後手歩9 */
                        kyoku->koma[i] = make_koma(40,8,9,3,1,2,1);
                        kyoku->ban.masu[9][3] = 40;
                }
        }
}

void make_komadai(KYOKUMEN* kyoku){

        int i,j;
        for(i=0;i<KOMADAI_NUM;i++){
                for(j=0;j<TEMOTI_MAX_NUM;j++){
                        kyoku->komadai.komadai[i][j] = 0;
                }
        }
}
int initialize(KYOKUMEN* kyoku){
        //id,駒番号,横,縦,先手後手、盤上成不成、価値
        //駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8

        int i;

        /* 駒、盤、駒台を作る */
        make_ban(kyoku);
        make_komadai(kyoku);
        init_koma_ban(kyoku);
        BanHyouji(kyoku);

        return 0;
}

int uninitialize(KYOKUMEN* kyoku){

        int i;

        for(i=1;i<=KOMA_NUM;i++){
                free(kyoku->koma[i]);
        }
        free(kyoku);
        return 0;
}

int main(void){
        int ret;

        /* 駒、盤、局面の作成 */

        KYOKUMEN* kyoku;
        kyoku = (KYOKUMEN *)malloc(sizeof(KYOKUMEN));
        if(kyoku == NULL){
                fprintf(stdout,"KYOKUMENを確保できませんでした\n");
                exit(1);
        }
        ret = initialize(kyoku);
        play(kyoku);
        ret = uninitialize(kyoku);
        return 0;
}

