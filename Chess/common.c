#include "common.h"

char*GetColor(Color color){
	switch(color){
		case white:
			return "White";
		break;
		case black:
			return "Black";
		break;
	}
	return "None";
}
bool IsPosEqual(Position a, Position b){
	if(a.x==b.x&&a.y==b.y)return true;
	return false;
}
void SetPosEqual(Position *a, Position b){
	a->x=b.x;
	a->y=b.y;
}
bool IsInBound(int x, int y){
	if(x<0||x>7||y<0||y>7) return false;
	return true;
}