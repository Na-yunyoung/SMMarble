#ifndef smm_object_h
#define smm_object_h

/* node type :
    lecture,
    restaurant,
    laboratory,
    home,
    experiment,
    foodChance,
    festival
*/


/* grade :
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/



//object generation
void smmObj_genNode(char* name, int type, int credit, int energy);
 

//member retrieving
char* smmObj_getNodeName(int node_nr);
int smmObj_getNodeType(int node_nr);


//element to string
char* smmObj_getTypeName(int type);
char* smmObj_getGradeName(int grade);


#endif /* smm_object_h */
