#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CRecord *g_pRecord = NULL;
char *spell_name[] = {
	"夜符「ナイトバード」",
	"闇符「ダークマター」",
	"月符「ムーンライトレイ」",
	"蛍符「ゲンジボタル」",
	"虫符「ナナホシテントウ」",
	"虫符「チョウセンカマキリ」",
	"蟲符「トビズムカデ」",
	"蟲符「オニグモ」",
	"蛙符「３／１アタック」",
	"冷符「セルシアス・ゼロ」",
	"氷符「アイスソード」",
	"霜符「フロストボックス」",
	"花符「フローズンフラワー」",
	"雪符「コッホスノーフレーク」",
	"鳥居「神社結界」",
	"横符「ホリゾナルストライク」",
	"縦符「ヴァーティカルブレイク」",
	"鳥目「レミラーマ」",
	"酉符「真夜中のマスタースパーク」",
	"圓符「ミステリーサークル」",
	"陰陽「太極変化」",
	"三精「日月星の共輝」",
	"四方「龍虎雀武」",
	"五行「桔梗清明」",
	"六芒「篭目包囲」",
	"七神「招福仙船」",
	"八卦「西遊悟空」",
	"囲符「井の中の蛙」",
	"白符「ホワイトアルバム」",
	"凍符「アブソリュート・ゼロ」",
	"災厄「ハリケーン・カトリーナ」",
	"災厄「ツナミパニック」",
	"災厄「アースクエイク」",
	"人災「オゾンホール」",
	"人災「グローバルワーミング」",
	"人災「ニュークリアウィンター」"
};

CRecord::CRecord(){
	::ZeroMemory(&data,sizeof(data));
	Load();
}
CRecord::~CRecord(){
	Save();
}
void CRecord::Save(){
	WriteFile( "score.dat", &data, sizeof(data));
}
void CRecord::Load(){
	ReadFile( "score.dat", &data, sizeof(data));
}
