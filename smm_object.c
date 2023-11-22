#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


//Node type 정의 
static char smmNodeName[MAX_TYPE_MAX][MAX_CHARNAME] = {
	"강의",
	"식당",
	"실험실",
	"집",
	"실험실로 이동",
	"음식찬스",
	"축제기간"
};

//node type에 해당하는 이름 반환하는 함수 정의 
char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
}



//object의 정보를 저장하는 변수 선언 
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode = 0;



//object generation
void smmObj_genNode(char* name, int type, int credit, int energy) //노드 만드는 함수 
{
	//object에 정보 저장 
	strcpy(smmObj_name[smmObj__noNode],name); 
	smmObj_type[smmObj_noNode] = type;
	smmObj_credit[smmObj_noNode] = credit;
	smmObj_energy[smmObj_noNode] = energy;
	
	smmObj_noNode++
}


//object 번호에 해당하는 이름반환 
char* smmObj_gotNodeName(int node_nr)
{
	return smmObj_name[node_nr];
}

// object 번호에 해당하는 타입반환 
int smmObj_gotNodeType(int node_nr)
{
	return smmObj_type[node_nr];
 } 


//element to string
char* smmObj_getNodeName(int type)
{
    return smmNodeName[type];
}

char* smmObj_getGradeName(int grade)
{
    return smmGradeName[grade];
}

