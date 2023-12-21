#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"
#define MAX_NODE        100
#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;
static int player_nr;

//player struct ���� 
typedef struct player {
    int energy;
    int position;
    char name[MAX_CHARNAME];
    int accumCredit;
    int flag_graduate[8];
} player_t;


static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
//void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
//void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
void smmGrade_e takeLecture(int player, char* lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char* lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif

void printGrades(int player) {
    int i;

    printf("������ ����� ���\n");

    for (i = 0; i < 8; i++) {
        if (cur_player[player].flag_graduate[i] == 0)
            break;
        else {
            printf("%s\n", smmObj_getNodeName(cur_player[player].flag_graduate[i]));
        }
    }
}

//player ���� ��� �Լ� 
void printPlayerStatus(void)
{
    int i;

    for (i = 0; i < player_nr; i++)
    {
        printf("%8s : credit %2i, energy %3i, position %2i\n",
            cur_player[i].name,
            cur_player[i].accumCredit,
            cur_player[i].energy,
            cur_player[i].position);
    }
}

//�ʱⰪ ���� �� input player name 
void generatePlayers(int n, int initEnergy) //generate a new player
{
    int i, j;
    //n time loop
    for (i = 0; i < n; i++)
    {
        //input name
        printf("Input player %i's name:", i);
        scanf("%s", cur_player[i].name);
        fflush(stdin);

        //set position
        //player_position[i] = 0;
        cur_player[i].position = 0;

        //set energy
        //player_energy[i] = initEnergy;
        cur_player[i].energy = initEnergy;
        cur_player[i].accumCredit = 0;

        for (j = 0; j < 8; j++)
            cur_player[i].flag_graduate[j] = 0;
    }
}

//random dice
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);


     if (c == 'g')
         printGrades(player);

    return (rand() % MAX_DIE + 1);
}

//action code when a player stays at a node
void actionNode(int player, int die_result)
{
    int type = smmObj_getNodeType(cur_player[player].position);
    char choice_lecture;
    int restaurant_node;
    int random_food;
    int random_mission;
    int i;
    int lab_dice;
    int standard_lab;
    int flag = 0;

    srand(time(NULL));

    switch (type)
    {
        //case lecture:
    case SMMNODE_TYPE_LECTURE:
        for (i = 0; i < 8; i++) {
            if (cur_player[player].flag_graduate[i] == cur_player[player].position % 16) {
                flag = 1;
                break;
            }
        }

        if (flag == 1) {
            printf("�̹� ������ �����Դϴ�!\n");
        }
        else if (cur_player[player].energy >= smmObj_getNodeCredit(cur_player[player].position))
        {
            printf("�����ϱ�-> y , ����ϱ�-> n : ");
            scanf("%c", &choice_lecture);
            fflush(stdin);

            if (choice_lecture == 'y')//choice_lecture : yes 
            {
                printf("�����մϴ�.\n");
                cur_player[player].accumCredit += smmObj_getNodeCredit(cur_player[player].position);
                cur_player[player].energy -= smmObj_getNodeEnergy(cur_player[player].position);

                //���� ��Ͽ� ��� ��ȣ�� �̿��ؼ� ����
                for (i = 0; i < 8; i++) {
                    if (cur_player[player].flag_graduate[i] == 0) {
                        cur_player[player].flag_graduate[i] = cur_player[player].position % 16;
                        break;
                    }
                    else
                        continue;
                }
            }
            else
                printf("����մϴ�.\n");//choice_lecture : drop
        }
        else
            printf("Not enough Energy!\n"); // ������ ���� ����� �������� ���� ��� 
        break;

        //case restaurant:
    case SMMNODE_TYPE_RESTAURANT:
        restaurant_node = cur_player[player].position;

        cur_player[player].energy += smmObj_getNodeEnergy(restaurant_node);
        break;

        //case home :
    case SMMNODE_TYPE_HOME:
        if ((cur_player[player].position % 16 == 0) && (cur_player[player].position != 0)) //�� node�� �� ��� 
        {
            cur_player[player].energy += smmObj_getNodeEnergy(0);
            printf("���� �����Ͽ� �������� ����ϴ�.\n");
        }

        break;

        //case food chance:
    case SMMNODE_TYPE_FOODCHANCE:
        random_food = rand() % 14;//random food ���� 

        cur_player[player].energy += smmObj_getFoodEnergy(random_food);
        printf("%s��(��) �Ծ����ϴ�.\n", smmObj_getFoodName(random_food));
        break;

        //case gotolab 
    case SMMNODE_TYPE_GOTOLAB:
        if (cur_player[player].position % 16 == 8 || cur_player[player].position % 16 == 12) //case experiment ����ǿ��� �����ϱ�
        {
            if (cur_player[player].position % 16 == 12)
            {
                printf("������ ���·� ��ȯ�ϸ� ����Ƿ� �̵��մϴ�.\n");
                cur_player[player].position -= 4;  //����ð�(12 node)�� �Ǿ �����(8 node)�� �̵��ϴ� ���̽�
            }
            printf("���� ���� ���ذ��� �Է��Ͻÿ� 1-6���� �� �ϳ�: ");
            scanf("%i", &standard_lab);
            fflush(stdin);
            for (i = 0; i < MAX_NODE; i++)
            {
                cur_player[player].energy -= smmObj_getNodeEnergy(cur_player[player].position);//�����ϴ� ���� ������ �Ҹ� 
                lab_dice = (rand() % MAX_DIE + 1);
                if (lab_dice >= standard_lab)// experiment finish
                {
                    printf("finish experiment!!!\n");
                    printf("%i��ŭ �ֻ����� ���Ƚ��ϴ�.\n", lab_dice);
                    break;
                }
                else // experiment fail
                {
                    printf("%i��ŭ �ֻ����� ���Ƚ��ϴ�.\n", lab_dice);
                    printf("fail experiment again....\n");

                    continue;
                }

            }

        }
        break;



        //case festival mission:
    case SMMNODE_TYPE_FESTIVAL:
        random_mission = rand() % 5; //random mission ���� 

        if (cur_player[player].position % 16 == 10)
        {
            printf("====================mission start====================\n");
            printf("'%s'\n", smmObj_getfestivalName(random_mission));
            printf("=====================================================\n");
        }
        break;

    default:
        break;
    }

    if (cur_player[player].position % 16 != 0 && cur_player[player].position % 16 - die_result < 0)
    {
        cur_player[player].energy += smmObj_getNodeEnergy(0);
        printf("���� �����ļ� �������� ����ϴ�.\n");
    }

}

//player�� ������ 
void goForward(int player, int step)
{
    cur_player[player].position += step;

    printf("%s go to node %i (name: %s)\n",
        cur_player[player].name, cur_player[player].position,
        smmObj_getNodeName(cur_player[player].position));//�̵��� node name 
}


int main(int argc, const char* argv[]) {

    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;


    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;

    srand(time(NULL));


    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        return -1;
    }

    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        smmObj_genNode(name, type, credit, energy);
        if (type == SMMNODE_TYPE_HOME)
            initEnergy = energy;
        board_nr++;

    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);

    //��� �ȳ� 
    for (i = 0; i < board_nr; i++)
        printf("node %2i : %15s, %i(%s), credit %i, energy %i\n",
            i, smmObj_getNodeName(i),
            smmObj_getNodeType(i), smmObj_getTypeName(smmObj_getNodeType(i)),
            smmObj_getNodeCredit(i), smmObj_getNodeEnergy(i));

    //    printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));


        //2. food card config 
    if ((fp = fopen(FOODFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }

    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        smmObj_genfood(name, energy);
        food_nr++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);

    for (i = 0; i < food_nr; i++)
        printf("food %2i : %8s, energy %i\n", i, smmObj_getFoodName(i), smmObj_getFoodEnergy(i));


    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH, "r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }

    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        smmObj_genfestival(name);
        festival_nr++;//store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    for (i = 0; i < festival_nr; i++)
    {
        printf("%s\n", smmObj_getfestivalName(i));
    }

    printf("\n\n");

    //2. Player configuration ---------------------------------------------------------------------------------

    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    } while (player_nr < 0 || player_nr >  MAX_PLAYER);

    generatePlayers(player_nr, initEnergy);

    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        int flag = 0;

        //4-1. initial printing
        printPlayerStatus();

        //4-2. die rolling (if not in experiment)        
        die_result = rolldie(turn);

        //4-3. go forward
        goForward(turn, die_result);

        //4-4. take action at the destination node of the board
        actionNode(turn, die_result);

        //4-5. next turn
        turn = (turn + 1) % player_nr;

        //���� ���� ����
        for (i = 0; i < player_nr; i++) {
            if (cur_player[i].position >= MAX_NODE)
                flag = 1;
            else if (cur_player[i].accumCredit >= GRADUATE_CREDIT)
                flag = 2;
        }

        if (flag==1)
        {
            printf("player�� node�� ������ϴ�. ��������\n");//node���� player�� �� ���� �� ���
            break;
        }
        else if (flag == 2)
        {
            printf("���������� �̼��Ͽ����ϴ�. �������� ������ ���ÿ�\n");//���������� �̼��� ��� 
            printf("%s��(��) ������ ����\n", cur_player[turn].name);
            for (i = 0; i < 8; i++) {
                if (cur_player[turn].flag_graduate[i] == 0)
                    break;
                else {
                    printf("%s\n", smmObj_getNodeName(cur_player[turn].flag_graduate[i]));
                    printf("total credit : %i\n",cur_player[turn].accumCredit);
                }
            }
            break;
        }


    }

    system("PAUSE");
    return 0;
}
