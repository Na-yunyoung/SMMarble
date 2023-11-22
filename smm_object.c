#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


//Node type ���� 
static char smmNodeName[MAX_TYPE_MAX][MAX_CHARNAME] = {
	"����",
	"�Ĵ�",
	"�����",
	"��",
	"����Ƿ� �̵�",
	"��������",
	"�����Ⱓ"
};

//node type�� �ش��ϴ� �̸� ��ȯ�ϴ� �Լ� ���� 
char* smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
}



//object�� ������ �����ϴ� ���� ���� 
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode = 0;



//object generation
void smmObj_genNode(char* name, int type, int credit, int energy) //��� ����� �Լ� 
{
	//object�� ���� ���� 
	strcpy(smmObj_name[smmObj__noNode],name); 
	smmObj_type[smmObj_noNode] = type;
	smmObj_credit[smmObj_noNode] = credit;
	smmObj_energy[smmObj_noNode] = energy;
	
	smmObj_noNode++
}


//object ��ȣ�� �ش��ϴ� �̸���ȯ 
char* smmObj_gotNodeName(int node_nr)
{
	return smmObj_name[node_nr];
}

// object ��ȣ�� �ش��ϴ� Ÿ�Թ�ȯ 
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

