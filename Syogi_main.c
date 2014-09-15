#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFSIZE 1024
#define MASUME_NUM 10
#define KOMA_NUM 40
#define KOMADAI_NUM 2
#define TEMOTI_MAX_NUM 40
#define GET_KOMA_LABEL(k, i, j) \
	KOMA_LABEL[(k)->koma[(k)->ban.masu[(j)][(i)]]->koma_num]
#define SENTE_FUNARI	"\033[40m\033[37m%s\033[39m\033[49m"
#define SENTE_NARI		"\033[40m\033[31m%s\033[39m\033[49m"
#define GOTE_FUNARI		"\033[47m\033[30m%s\033[39m\033[49m"
#define GOTE_NARI		"\033[41m\033[30m%s\033[39m\033[49m"


/* TODO 裏は？ */
typedef enum __KOMA_TYPE
{
	TYPE_KARA,
	TYPE_OH,
	TYPE_HI,
	TYPE_KAKU,
	TYPE_KIN,
	TYPE_GIN,
	TYPE_KEI,
	TYPE_KYO,
	TYPE_FU
} KOMA_TYPE;

static char *KOMA_LABEL[] =
{
	"　",
	"玉",
	"飛",
	"角",
	"金",
	"銀",
	"桂",
	"香",
	"歩"
};

static char *YOKO_LABEL[] =
{
	"",
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
	"七",
	"八",
	"九"
};

typedef struct __KOMA{
	int id;
	KOMA_TYPE koma_num; //駒番号一覧　玉:1 飛:2 角:3 金:4 銀:5 桂:6 香:7 歩:8
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
int Sitei_Koma_check(KYOKUMEN* kyoku,int yoko,int tate){
	int id;

	id = kyoku->ban.masu[yoko][tate];
	if(id == 0)
	{
		fprintf(stdout,"そこにはコマは存在しません\n");
		return -1;
	}

	if(kyoku->koma[id]->sente_gote != kyoku->sente_gote)
	{
		fprintf(stdout,"指定されたコマは相手のコマです\n");
		return -1;
	}

	/* 自分のコマである */
	return 0;
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
	int	i;
	int	j;

	/* 行ループ */
	for (i = 0; i < MASUME_NUM; i++)
	{
		/* 列ループ:行番号と駒を表示する。 */
		for (j = MASUME_NUM - 1; j > 0; j--)
		{
			if (i == 0)
			{
				/* 行番号 */
				fprintf(stdout, "%4d %s", j,
					j == 1 ? "\n" : "");
			}
			else
			{
				KOMA   *koma;

				/* 駒 */
				koma = kyoku->koma[kyoku->ban.masu[j][i]];
				if (koma->sente_gote == 0 &&
					koma->nari_funari == 2)
				{
					fprintf(stdout, "| " SENTE_FUNARI " ",
						GET_KOMA_LABEL(kyoku, i, j));
				}
				else if (koma->sente_gote == 0 &&
					 koma->nari_funari == 1)
				{
					fprintf(stdout, "| " SENTE_NARI " ",
						GET_KOMA_LABEL(kyoku, i, j));
				}
				else if (koma->sente_gote == 1 &&
					 koma->nari_funari == 2)
				{
					fprintf(stdout, "| " GOTE_FUNARI " ",
						GET_KOMA_LABEL(kyoku, i, j));
				}
				else if (koma->sente_gote == 1 &&
					 koma->nari_funari == 1)
				{
					fprintf(stdout, "| " GOTE_NARI " ",
						GET_KOMA_LABEL(kyoku, i, j));
				}
				else
				{
					fprintf(stdout, "| %s " ,
						GET_KOMA_LABEL(kyoku, i, j));
				}

				if (j == 1)
				{
					fprintf(stdout, "| %s\n",
						YOKO_LABEL[i]);
				}
			}
		}

		/* 列ループ:盤線を表示する。 */
		for (j = MASUME_NUM - 1; j > 0; j--)
		{
			fprintf(stdout, "+----%s", j == 1 ? "+\n" : "");
		}
	}

	/* 手持ちのコマを表示する */
	for(i=1;i<=KOMA_NUM;i++)
	{
		if(kyoku->koma[i]->nari_funari == 0)
		{
			if(kyoku->koma[i]->sente_gote == 0)
			{
				fprintf(stdout,"先手駒台:%d:" SENTE_FUNARI "\n",
					kyoku->koma[i]->id,
					KOMA_LABEL[kyoku->koma[i]->koma_num]);
			}
			else if(kyoku->koma[i]->sente_gote == 1)
			{
				fprintf(stdout,"	   後手駒台:%d:" GOTE_FUNARI "\n",
					kyoku->koma[i]->id,
					KOMA_LABEL[kyoku->koma[i]->koma_num]);
			}
		}
	}

	if(kyoku->sente_gote == 0){
		fprintf(stdout,"先手番です\n");
	}else if(kyoku->sente_gote == 1){
		fprintf(stdout,"後手番です\n");
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

int
ScanInt(char *message, int *val, int min, int max)
{
	char	buf[MAX_BUFSIZE];
	char   *endptr;

	while (1)
	{
		fprintf(stdout, "%s", message);

		if (fgets(buf, MAX_BUFSIZE, stdin) == NULL)
		{
			return -1;
		}

		if (strlen(buf) >= MAX_BUFSIZE - 1)
		{
			return -1;
		}

		*val = strtol(buf, &endptr, 10);
		if (buf == endptr)
		{
			fprintf(stdout,"有効な数値を指定してください\n");
			continue;
		}

		if (*val < min)
		{
			fprintf(stdout,"%d以上の数値を指定してください\n", min);
			continue;
		}

		if (*val > max)
		{
			fprintf(stdout,"%d以下の数値を指定してください\n", max);
			continue;
		}

		break;
	}

	return 0;
}

/* コマを打つ */
int Utu(KYOKUMEN* kyoku){
	int i,id,yoko,tate,ret;

	/* 駒番号読み込み */
	if(ScanInt("打つコマを指定してください:", &id, 0, KOMA_NUM) < 0){
		return -1;
	}
	if(kyoku->komadai.komadai[kyoku->sente_gote][id] == 0){
		fprintf(stdout,"その駒は駒台にはありません\n");
		return -1;
	}
	fprintf(stdout,"どこに置きますか?\n");
	if(ScanInt("横:", &yoko, 1, 10) < 0){
		return -1;
	}
	if((yoko < 1) || (10 < yoko)){
		fprintf(stdout,"そこにコマは置けません\n");
		return -1;
	}
	if(ScanInt("縦:", &tate, 1, 10) < 0){
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
int Ugokasu(KYOKUMEN* kyoku){

	int id;
	int yoko_before;
	int tate_before;
	int yoko_after;
	int tate_after;
	int ret;

	/*
	 * 動かす駒の縦横の位置を受け付けて、その位置に自身の駒が配置してあった
	 * ら動かす駒の候補として確定する。
	 * 縦横ともに0が指定されたら、駒を打つ。
	 */
	while(1)
	{
		fprintf(stdout,"動かす駒を指定してください(0 0で駒を打ちます)\n");
		if (ScanInt("横:", &yoko_before, 0, 9) < 0 ||
			ScanInt("縦:", &tate_before, 0, 9) < 0)
		{
			return -1;
		}

		/* 0 0が指定されたらコマを打つ */
		if((yoko_before == 0) && (tate_before == 0)){
			if( Utu(kyoku) == -1){
				continue;
			}else{
				goto Utta;
			}
		}

		/* 指定された駒がそこにあるかを判定する */
		if (Sitei_Koma_check(kyoku,yoko_before,tate_before) == 0)
		{
			break;
		}
	}

	/* 動かすコマのIDを特定する */
	id = kyoku->ban.masu[yoko_before][tate_before];

	while(1)
	{
		/* どこに */
		fprintf(stdout,"動かす場所を指定してください\n");
		if (ScanInt("横:", &yoko_after, 1, 9) < 0 ||
			ScanInt("縦:", &tate_after, 1, 9) < 0)
		{
			return -1;
		}

		/* 指定された駒が、指定した場所に動けるか判定する */
		ret = Ugoku_Hanni_Hantei(kyoku,yoko_before,tate_before,yoko_after,tate_after,id);
		/* 範囲が間違っていてコマを動かせない */
		if(ret == -1)
		{
			continue;
		}

		/*
		 * 範囲はあっている。
		 * 駒が動かす先にあるかを判定する。
		 * 同時にその駒の敵味方も判定する)
		 */
		ret = Koma_Sonzai_Check(kyoku,yoko_after,tate_after);
		/*
		* 戻り値0:そこに駒はない。
		* 戻り値-1:その駒は敵のコマである。
		* 戻り値-2:その駒は味方のコマである。
		 */
		if(ret != 0){
			if(ret == -1){
			/* 敵のコマなので取る!!!!!! */
				kyoku = Toru(kyoku,yoko_after,tate_after);
				break;
			}
			else if(ret == -2){
				fprintf(stdout,"取ろうとしている駒は味方のものです\n");
				continue;
			}
		}else if(ret == 0){
		/* 動かせる！！！！！！ */
			break;
		}
	}

	/* 指定した場所にコマを動かす */
	kyoku->ban.masu[yoko_after][tate_after] = id;
	kyoku->ban.masu[yoko_before][tate_before] = 0;
	kyoku->koma[id]->yoko = yoko_after;
	kyoku->koma[id]->tate = tate_after;

	/* なるかどうかを決める*/
	if (((kyoku->koma[id]->sente_gote == 0 && tate_after <= 3) ||
		 (kyoku->koma[id]->sente_gote == 1 && tate_after >= 7)) &&
		 kyoku->koma[id]->nari_funari == 2)
	{
		int	tmp;

		if(ScanInt("成りますか？成:1 不成:2:", &tmp, 1, 2) < 0)
		{
			return -1;
		}

		if(tmp == 1)
		{
			kyoku->koma[id]->nari_funari = 1;
		}
	}

	/* 盤表示をする*/
	BanHyouji(kyoku);
	return 0;

	/* 打った */
	Utta:
	BanHyouji(kyoku);
	return 0;
}

/* ゲームの実行 */
void play(KYOKUMEN* kyoku){
	int	val;

	kyoku->sente_gote = 0;//0:先手 1:後手

	while(1){
		if(ScanInt("続行:0 終了:1:", &val, 0, 1) < 0)
		{
			break;
		}

		/* 終了 */
		if(val == 1)
		{
			if(ScanInt("本当に？続行:0 終了:1:", &val, 0, 1) < 0)
			{
				break;
			}

			if(val == 1)
			{
				break;
			}
		}

		/* 一手分プレイする */
		BanHyouji(kyoku);
		if (Ugokasu(kyoku) < 0)
			break;

		/* 先手後手を入れ替える */
		if(kyoku->sente_gote == 0){
			kyoku->sente_gote = 1;
		}else if(kyoku->sente_gote == 1){
			kyoku->sente_gote = 0;
		}
	}
}

/* 駒を作る */
KOMA* make_koma(int id,KOMA_TYPE koma_num,int yoko,int tate,int sente_gote,int nari_funari,int kachi){

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

	for(i=0;i<=KOMA_NUM;i++){
		if(i==0){
			/* 空 */
			kyoku->koma[i] = make_koma(i,TYPE_KARA,0,0,-1,-1,-1);
		}else if(i==1){
		/* 先手玉 */
			kyoku->koma[i] = make_koma(1,TYPE_OH,5,9,0,2,200);
			kyoku->ban.masu[5][9] = 1;
		}else if(i==2){
		/* 後手玉 */
			kyoku->koma[i] = make_koma(2,TYPE_OH,5,1,1,2,200);
			kyoku->ban.masu[5][1] = 2;
		}else if(i==3){
			/* 先手飛 */
			kyoku->koma[i] = make_koma(3,TYPE_HI,2,8,0,2,50);
			kyoku->ban.masu[2][8] = 3;
		}else if(i==4){
			/* 後手飛 */
			kyoku->koma[i] = make_koma(4,TYPE_HI,8,2,1,2,50);
			kyoku->ban.masu[8][2] = 4;
		}else if(i==5){
			/* 先手角 */
			kyoku->koma[i] = make_koma(5,TYPE_KAKU,8,8,0,2,50);
			kyoku->ban.masu[8][8] = 5;
		}else if(i==6){
			/* 後手角 */
			kyoku->koma[i] = make_koma(6,TYPE_KAKU,2,2,1,2,50);
			kyoku->ban.masu[2][2] = 6;
		}else if(i==7){
			/* 先手金1 */
			kyoku->koma[i] = make_koma(7,TYPE_KIN,4,9,0,2,40);
			kyoku->ban.masu[4][9] = 7;
		}else if(i==8){
			/* 先手金2 */
			kyoku->koma[i] = make_koma(8,TYPE_KIN,6,9,0,2,40);
			kyoku->ban.masu[6][9] = 8;
		}else if(i==9){
			/* 後手金3 */
			kyoku->koma[i] = make_koma(9,TYPE_KIN,4,1,1,2,40);
			kyoku->ban.masu[4][1] = 9;
		}else if(i==10){
			/* 後手金4 */
			kyoku->koma[i] = make_koma(10,TYPE_KIN,6,1,1,2,40);
			kyoku->ban.masu[6][1] = 10;
		}else if(i==11){
			/* 先手銀1 */
			kyoku->koma[i] = make_koma(11,TYPE_GIN,3,9,0,2,30);
			kyoku->ban.masu[3][9] = 11;
		}else if(i==12){
			/* 先手銀2 */
			kyoku->koma[i] = make_koma(12,TYPE_GIN,7,9,0,2,30);
			kyoku->ban.masu[7][9] = 12;
		}else if(i==13){
			/* 後手銀3 */
			kyoku->koma[i] = make_koma(13,TYPE_GIN,3,1,1,2,30);
			kyoku->ban.masu[3][1] = 13;
		}else if(i==14){
			/* 後手銀4 */
			kyoku->koma[i] = make_koma(14,TYPE_GIN,7,1,1,2,30);
			kyoku->ban.masu[7][1] = 14;
		}else if(i==15){
			/* 先手桂1 */
			kyoku->koma[i] = make_koma(15,TYPE_KEI,2,9,0,2,20);
			kyoku->ban.masu[2][9] = 15;
		}else if(i==16){
			/* 先手桂2 */
			kyoku->koma[i] = make_koma(16,TYPE_KEI,8,9,0,2,20);
			kyoku->ban.masu[8][9] = 16;
		}else if(i==17){
			/* 後手桂3 */
			kyoku->koma[i] = make_koma(17,TYPE_KEI,2,1,1,2,20);
			kyoku->ban.masu[2][1] = 17;
		}else if(i==18){
			/* 後手桂4 */
			kyoku->koma[i] = make_koma(18,TYPE_KEI,8,1,1,2,20);
			kyoku->ban.masu[8][1] = 18;
		}else if(i==19){
			/* 先手香1 */
			kyoku->koma[i] = make_koma(19,TYPE_KYO,1,9,0,2,10);
			kyoku->ban.masu[1][9] = 19;
		}else if(i==20){
			/* 先手香2 */
			kyoku->koma[i] = make_koma(20,TYPE_KYO,9,9,0,2,10);
			kyoku->ban.masu[9][9] = 20;
		}else if(i==21){
			/* 後手香3 */
			kyoku->koma[i] = make_koma(21,TYPE_KYO,1,1,1,2,10);
			kyoku->ban.masu[1][1] = 21;
		}else if(i==22){
			/* 後手香4 */
			kyoku->koma[i] = make_koma(22,TYPE_KYO,9,1,1,2,10);
			kyoku->ban.masu[9][1] = 22;
		}else if(i==23){
			/* 先手歩1 */
			kyoku->koma[i] = make_koma(23,TYPE_FU,1,7,0,2,1);
			kyoku->ban.masu[1][7] = 23;
		}else if(i==24){
			/* 先手歩2 */
			kyoku->koma[i] = make_koma(24,TYPE_FU,2,7,0,2,1);
			kyoku->ban.masu[2][7] = 24;
		}else if(i==25){
		/* 先手歩3 */
			kyoku->koma[i] = make_koma(25,TYPE_FU,3,7,0,2,1);
			kyoku->ban.masu[3][7] = 25;
		}else if(i==26){
		/* 先手歩4 */
			kyoku->koma[i] = make_koma(26,TYPE_FU,4,7,0,2,1);
			kyoku->ban.masu[4][7] = 26;
		}else if(i==27){
		/* 先手歩5 */
			kyoku->koma[i] = make_koma(27,TYPE_FU,5,7,0,2,1);
			kyoku->ban.masu[5][7] = 27;
		}else if(i==28){
		/* 先手歩6 */
			kyoku->koma[i] = make_koma(28,TYPE_FU,6,7,0,2,1);
			kyoku->ban.masu[6][7] = 28;
		}else if(i==29){
		/* 先手歩7 */
			kyoku->koma[i] = make_koma(29,TYPE_FU,7,7,0,2,1);
			kyoku->ban.masu[7][7] = 29;
		}else if(i==30){
		/* 先手歩8 */
			kyoku->koma[i] = make_koma(30,TYPE_FU,8,7,0,2,1);
			kyoku->ban.masu[8][7] = 30;
		}else if(i==31){
		/* 先手歩9 */
			kyoku->koma[i] = make_koma(31,TYPE_FU,9,7,0,2,1);
			kyoku->ban.masu[9][7] = 31;
		}else if(i==32){
		/* 後手歩1 */
			kyoku->koma[i] = make_koma(32,TYPE_FU,1,3,1,2,1);
			kyoku->ban.masu[1][3] = 32;
		}else if(i==33){
		/* 後手歩2 */
			kyoku->koma[i] = make_koma(33,TYPE_FU,2,3,1,2,1);
			kyoku->ban.masu[2][3] = 33;
		}else if(i==34){
		/* 後手歩3 */
			kyoku->koma[i] = make_koma(34,TYPE_FU,3,3,1,2,1);
			kyoku->ban.masu[3][3] = 34;
		}else if(i==35){
		/* 後手歩4 */
			kyoku->koma[i] = make_koma(35,TYPE_FU,4,3,1,2,1);
			kyoku->ban.masu[4][3] = 35;
		}else if(i==36){
		/* 後手歩5 */
			kyoku->koma[i] = make_koma(36,TYPE_FU,5,3,1,2,1);
			kyoku->ban.masu[5][3] = 36;
		}else if(i==37){
		/* 後手歩6 */
			kyoku->koma[i] = make_koma(37,TYPE_FU,6,3,1,2,1);
			kyoku->ban.masu[6][3] = 37;
		}else if(i==38){
		/* 後手歩7 */
			kyoku->koma[i] = make_koma(38,TYPE_FU,7,3,1,2,1);
			kyoku->ban.masu[7][3] = 38;
		}else if(i==39){
		/* 後手歩8 */
			kyoku->koma[i] = make_koma(39,TYPE_FU,8,3,1,2,1);
			kyoku->ban.masu[8][3] = 39;
		}else if(i==40){
		/* 後手歩9 */
			kyoku->koma[i] = make_koma(40,TYPE_FU,9,3,1,2,1);
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

	return 0;
}

int uninitialize(KYOKUMEN* kyoku){

	int i;

	for(i=0;i<=KOMA_NUM;i++){
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
