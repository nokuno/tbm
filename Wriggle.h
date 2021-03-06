#pragma once

//リグル共通
struct CBossWriggleBase : public CBossBase
{
	CBossWriggleBase();
	virtual int Draw();
	virtual int SpellCard();
	int DontCome();
};
//中ボス
struct CBossWriggleMiddle : public CBossWriggleBase
{
	CBossWriggleMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack1();
	int GenjiBotaru();
};
struct CBossWriggle : public CBossWriggleBase
{
	CBossWriggle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack2();
	int Tentou();
	int Kamakiri();
	int Mukade();
	int Kumo();
};
