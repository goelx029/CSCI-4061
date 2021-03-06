/*
 * VCforStudents.c
 *
 *  Created on: Feb 2, 2018
 *      Author: ayushi
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "makeargv.h"

#define MAX_NODES 100

//Function signatures
// Note : The use of exit(-1) : denotes that an error is being faced and the user needs to change something.

/*  login : goelx029, hartm433
*   date : 02/21/18
*   name : Saksham Goel, Franklin Hartman
*   id : goelx029(5138568), hartm433(5062484)
*/

//***************************************************************************//
// Get Candidate Information from the Line 1 Using these functions below //
//***************************************************************************//

int getNumOfCandidates(char *buf) {
	char ** argvp = NULL; //creating array of arguments, which will contain the number of candidates at the 0 index.
	if (makeargv(trimwhitespace(buf), " ", &argvp) == 0) { //error handling
		printf("%s\n", "The first line does not contain data in the correct format. Please make sure it is correct.");
		exit(-1);
	} else {
		int num = atoi(argvp[0]); //changing string input to int
		if (num == 0) { //error handling
			printf("%s\n", "The first line does not contain a number at starting. Please change the input file.");
			exit(-1);
		}
		return num; //return number of candidates
	}
}

char * getStringNumOfCandidates(char *line) {
	//identical to getNumOfCandidates but does not change string input to int
	char ** argvp = NULL;
	if (makeargv(trimwhitespace(line), " ", &argvp) == 0) { //error handling
		printf("%s\n", "The first line does not contain data in the correct format. Please make sure it is correct.");
		exit(-1);
	} else {
		int num = atoi(argvp[0]);
		if (num == 0) {
			printf("%s\n", "The first line does not contain a number at starting. Please change the input file.");
			exit(-1);
		}
		return argvp[0];
	}
}

char * getStringOfCandidates(char *line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp); //len is the number of candidates + 1
	if ( len == 0) {
		printf("%s\n", "The first line does not contain data in the correct format. Please make sure it is correct.");
		exit(-1);
	} else {
		if (len != atoi(argvp[0]) + 1) {
			printf("%s\n", "The number of candidates given does not satisfy the number of candidate names specified in the input file.");
			exit(-1);
		} else {
			//checking all-pairs of names for duplicates
			for (int i = 1; i < len; i++) {
				for (int j = i+1; j < len; j++) {
					if (strcmp(argvp[i], argvp[j]) == 0) {
						printf("%s\n", "The candidates have duplicate names. Currently the system does not support having duplicates candidate names.");
						exit(-1);
					}
				}
			}
			char * candstring = (char *) malloc(sizeof(char) * 1024); //allocate memory
			strcpy(candstring, ""); //initializing string to ""
			for (int i = 1; i < len;  i++) { //going through each candidate and adding them to a string (separated by a space)
				strcat(candstring, argvp[i]);
				strcat(candstring, " ");
			}
			trimwhitespace(candstring);
			return candstring;
		}
	}
}

//***************************************************************************//
//***************************************************************************//


//***************************************************************************//
// Get Regions Information from the Line 2 Using these functions below //
//***************************************************************************//

int checkValidNodes(char* line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp); //checking line 2 for number of nodes

	if (len == 0) { //error handling
		printf("%s\n", "The number of nodes given equal to zero. Please edit the input file.");
		exit(-1);
	} else {
		for (int i = 0; i < len; i++) {
			for (int j = i+1; j < len; j++) {
				if (strcmp(argvp[i], argvp[j]) == 0) {
					//checking all-pairs of nodes for duplicates
					printf("%s\n", "The nodes have duplicates. Currently the system does not support having duplicates node.");
					exit(-1);
				}
			}
		}
		for (int i = 0; i < len; i++) {
			if (strcmp(argvp[i], "Who_Won") == 0) {
				// Check whether Who_Won node exists. Need Who_Won to actualy run the program.
				return 0;
			}
		}
		return -1;
	}
}

int getNoOfNodes(char* line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp); //creating an int which will store the total number of nodes in the graph
	printf("Total number of Nodes in the DAG = %d\n\n", len);
	if (len < 2) { //if there is only one node, that will not work with the program.
		printf("The number of nodes given equal to %d. Should atleast have 2 or more nodes specified. Please edit the input file.\n", len);
		exit(-1);
	}
	return len;
}

struct node* getAllNodes(char* line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp); //int storing number of nodes

	struct node* nodes = (struct node *)malloc(len * sizeof(struct node)); //allocate memory

	if (len == 0) { //error handling for empty graph
		printf("%s\n", "The number of nodes given equal to zero. Please edit the input file.");
		exit(-1);
	} else {
		for(int i = 0; i < len; i++) { //creating node structs for each node in the graph
			char* str1 = "Output_";
			nodes[i] = (struct node){"", "", "", "", NULL, 0, 0, 0, 0};
			strcpy(nodes[i].name, argvp[i]);
			strcpy(nodes[i].output, argvp[i]);
			nodes[i].pid = i; //assigning process id's and id's based on position in loop
			nodes[i].id = i;
			prepend(nodes[i].output,str1);  //so that each node's output may start with "Output_" followed by the node name
		}
	}
	return nodes;
}
//***************************************************************************//
//***************************************************************************//


//***************************************************************************//
// Get Information about the nodes using these functions below //
//***************************************************************************//

int getID(struct node* nodes, int len, char name[]) {
	for (int i = 0; i < len; i++) {
		if (strcmp(nodes[i].name, name) == 0) { //len tells us how many nodes there are to parse through, and we return the id only if the name of the current node is equal to the input name
			return nodes[i].id;
		}
	}
	return -1;
}

char * getName(struct node* nodes, int len, int id) {
	//similar to getID, we parse through the graph until we have found the correct node. Although here we base it off the given id and return
	for (int i = 0; i < len; i++) {
		if (nodes[i].id == id) {
			return nodes[i].name;
		}
	}
	return "<NotFound>";
}

struct node * getNode(struct node* nodes, int len, int id) {
	//similar to getName, but returns the node itself rather than its name.
	for (int i = 0; i < len; i++) {
		if (nodes[i].id == id) {
			return &nodes[i];
		}
	}
	return NULL;
}

//check whether a node exists with the name same as given name as parameter.
int checkValidNodeName(struct node* nodes, int len, char nodename[]) {
	for (int i = 0; i < len; i++) {
		if (strcmp(nodes[i].name, nodename) == 0) {
			return 0;
		}
	}
	return -1;//invalid
}
//***************************************************************************//
//***************************************************************************//



//***************************************************************************//
// Functions below help create the DAG using the functions above as helpers//
//***************************************************************************//

// function that adds a particuar child to a particular parent
void addNeighbor(struct node* nodes, int len, char parent[], char child[]) {
	if (checkValidNodeName(nodes, len, parent) == -1) { //error handling
		printf("%s %s\n", "The name of the parent is not valid. Not allowed. Parent-", parent);
		exit(-1);
		return;
	}
	for (int i = 0; i < len; i++) {
		if (strcmp(nodes[i].name, parent) == 0) { //checking to see if the node is a parent, since we want to add the child for this
			int id = getID(nodes, len, child);
			if (id == -1) {
				printf("%s %s\n", "The name of the child node is not valid (child will be ommited). Child-", child);
				return;
			}
			nodes[i].children[nodes[i].num_children] = id; //attaching the child node's id to the parent
			nodes[i].num_children += 1;
			return;
		}
	}
}

// function that adds al child to a particular parent mentioned in the line of the input file.
int addNeighbors(struct node* nodes, int len, char* line) {
	char ** argvp = NULL; //parent will be 0 index
	int temp = makeargv(trimwhitespace(line), ":", &argvp);
	if (temp < 2) { //error handling for the incorrect formatting
		printf("%s %s\n", "Current line does not follow the right format (will be omitted), Line -", line);
		return -1;
	}
	char* parent = trimwhitespace(argvp[0]);
	char ** arg = NULL;
	int noOfNeighbors = makeargv(trimwhitespace(argvp[1]), " ", &arg);
	if (noOfNeighbors == 0) {
		printf("%s %s\n", "Current line does not have information about neighbors (will be omitted), Line -", line);
		return -1;
	}
	for (int i = 0; i < noOfNeighbors; i++) { //attach all children to the parent
		addNeighbor(nodes, len, parent, trimwhitespace(arg[i]));
	}
	return 0;
}

// function that designates a executable name to the particular node.
void addProg(struct node* nodes, int len) {
	int flag = 0;
	for (int i = 0; i < len; i++) { //checking to see which program should be ran
		if(!flag && strcmp(getName(nodes, len, nodes[i].id), "Who_Won") == 0) {
			flag = 1;
			strcpy(nodes[i].prog, "./find_winner");
		} else if (nodes[i].num_children != 0) {
			strcpy(nodes[i].prog, "./aggregate_votes");
		} else {
			strcpy(nodes[i].prog, "./leafcounter");
		}
	}
}


// function that designates an input file name or input files names to the particular node.
void addInput(struct node* nodes, int len, struct node * currNode) {
	if (currNode->num_children == 0) {
		strcpy(currNode->input[0], currNode->name); //only need to run once if there are no children
	} else {
		for (int i = 0; i < currNode->num_children; i++) {
			strcpy(currNode->input[i], getNode(nodes, len, currNode->children[i])->output);
		}
	}
}


// function that goes through the whole DAG and use the above function to add input file names for each node.
void addInputs(struct node* nodes, int len) {
	for (int i = 0; i < len; i++) { //adding the inputs for every node in the DAG
		addInput(nodes, len, &nodes[i]);
	}
}
//***************************************************************************//
//***************************************************************************//


// special function that checks whether the Who_Won node actually has neighbhors. (Need that to determine the result of the election)
void checkForWhoWonHasNeighbors(struct node* nodes, int len) { //error handling
	for (int i = 0; i < len; i++) {
		if(strcmp(nodes[i].name, "Who_Won") == 0) {
			if(nodes[i].num_children == 0) {
				printf("%s\n", "Who_Won node doesnt have any children cannot be allowed.");
				exit(-1);
			} else {
				return;
			}
		}
	}
}



//***************************************************************************//
// Function to initialize the whole graph by setting the status of every node = 0//
//***************************************************************************//
void initializeGraph(struct node * nodes, int len) {
	for(int i = 0; i < len; i++) {
		nodes[i].status = 0;
		// 0 status represents not visited the node yet.
	}
}
//***************************************************************************//
//***************************************************************************//


//***************************************************************************//
// Functions to check whether there is a back cycle or not//
// implement a simple DFS algorithm through the whole DAG and check whether the neighbor is a non white node.
//***************************************************************************//
void backCycleCheckVisit(struct node * nodes, int len, struct node * currNode, char * numCand, char * candString, int * status) {
	currNode->status = 1; //grey node
	for(int i = 0; i < currNode->num_children; i++) {
		if (getNode(nodes, len, currNode->children[i])->status == 0) {
			backCycleCheckVisit(nodes, len, getNode(nodes, len, currNode->children[i]), numCand, candString, status);
		} else {
			printf("%s\n", "Back Cycle Detected. Please change the graph in the input.txt file so that there is no back cycle.");
			printf("The names of the parent node : %s, neighbor node : %s\n", currNode->name, getNode(nodes, len, currNode->children[i])->name);
			*status = -1;
			return;
		}
	}
	currNode->status = 2; // black node
}

void backCycleCheck(struct node * nodes, int len, char * numCand, char * candString, int * status) {
	// initialize graph
	initializeGraph(nodes, len);
	for(int i = 0; i < len; i++) {
		if (nodes[i].status == 0) {
			backCycleCheckVisit(nodes, len, &nodes[i], numCand, candString, status);
		}
	}
}
//***************************************************************************//
//***************************************************************************//




/**Function : execNodes
 * This function generates the particular exec call for a particular node of the graph.
 * Does a lot of preprocessing to get the right arguments for the executable in order.
 * This preprocessing is saved in a char *array and then passed to the execvp call to execute.
 * */
void execNodes(node_t *n, char * numCand, char * candString) {
	if (strcmp(n->prog, "./leafcounter") == 0) {
		// form - ./leafcounter input_file output_file number_of_candiate names_of_candidates
		char *args[5+atoi(numCand)];
		args[0] = n->prog;
		args[1] = n->input[0];
		args[2] = n->output;
		args[3] = numCand;
		char ** argvp = NULL;
		int temp = makeargv(trimwhitespace(candString), " ", &argvp);
		for (int i = 0; i < atoi(numCand); i++) {
			args[i+4] = argvp[i];
		}
		args[5+atoi(numCand)-1] = NULL;

		char mode[] = "0777";
    int i;
    i = strtol(mode, 0, 8);
    if (chmod ("leafcounter",i) < 0)  {	//getting the right permissions.
			printf("%s\n", "chmod command on leafcounter didnt work. Exiting the program.");
			exit(-1);
		}
		if(access( args[1], F_OK ) != -1 ) {
		  // file exists
		} else {
			printf("Input file - %s not found. Exiting the program.\n", args[1]);
			exit(-1);
		} 
		if (chmod (args[1],i) < 0)  {//getting the right permissions.
			printf("chmod command on input file - %s didnt work. Exiting the program.\n", args[1]);
			exit(-1);
		}

		printf("\n");
		printf("Node name : %s , Executing : %s\n", n->name, n->prog);
		execvp(args[0], args);
		perror("Error encountered while executing the process : ");
		for (int i = 0; i < 5+atoi(numCand); i++) {
			free(args[i]);
		}
	} else {
		// form - ./aggregate_votes number_of_input_file input_file1 input_file2 output_file number_of_candiate names_of_candidates
		// form - ./find_winner number_of_input_file input_file1 input_file2 output_file number_of_candiate names_of_candidates
		char mode[] = "0777";
    int i;
    i = strtol(mode, 0, 8);

		char *args[5 + n->num_children + atoi(numCand)];
		args[0] = n->prog;

		char num[10];
  	sprintf(num, "%d", n->num_children); 
		args[1] = num;

		for (int j = 0; j < n->num_children; j++) {
			args[j+2] = n->input[j];
			if(access( args[j+2], F_OK ) == -1 ) { //error handling checing whether the input file exists or not.
				printf("Input file - %s not found. Exiting the program.\n", args[j+2]);
				exit(-1);
			} 
			if (chmod (args[j+2],i) < 0)  {//getting the right permissions.
				printf("chmod command on input file - %s didnt work. Exiting the program.\n", args[j+2]);
				exit(-1);
			}
		}

		args[2+n->num_children] = n->output;
		args[3+n->num_children] = numCand;
		char ** argvp = NULL;
		int temp = makeargv(trimwhitespace(candString), " ", &argvp);
		for (int i = 0; i < atoi(numCand); i++) {
			args[i+4+n->num_children] = argvp[i];
		}
		args[5+n->num_children+atoi(numCand)-1] = NULL;


    if (chmod ("aggregate_votes",i) < 0)  {//getting the right permissions.
			printf("%s\n", "chmod command on aggregate_votes didnt work. Exiting the program.");
			exit(-1);
		}
		if (chmod ("find_winner",i) < 0)  {//getting the right permissions.
			printf("%s\n", "chmod command on find_winner didnt work. Exiting the program.");
			exit(-1);
		}

		printf("\n");
		printf("Node name : %s , Executing : %s\n", n->name, n->prog);
		execvp(args[0], args);
		perror("Error encountered while executing the process : ");
		for (int i = 0; i < 5 + n->num_children + atoi(numCand); i++) {
			free(args[i]);
		}
	}
}
//***************************************************************************//
//***************************************************************************//






//***************************************************************************//
// Functions that implement a DFS algorithm on the DAG and execute the nodes.//
//***************************************************************************//
void DFSVisit(struct node * nodes, int len, struct node * currNode, char * numCand, char * candString) {
	currNode->status = 1; //convert the white node to a grey node

	if (currNode->num_children == 0) {
		pid_t child_pid = fork();
		if (child_pid == 0) {
			//no need to do anything here since there are no children
		} else {
			wait(NULL);
			execNodes(currNode, numCand, candString);
		}
	} else {
		pid_t child_pid = fork();
		if (child_pid == 0) { // the child process is responsible to first go through all the nodes and execute them.
			for(int i = 0; i < currNode->num_children; i++) {
				if (getNode(nodes, len, currNode->children[i])->status == 0) {
					DFSVisit(nodes, len, getNode(nodes, len, currNode->children[i]), numCand, candString);
				}
			}
		} else { // once the child process has executed all the dependecies, the parent process can run the correct executable as it guarantees the output file.
			wait(NULL);
			execNodes(currNode, numCand, candString);
		}

		/*
		* Another way of doing the voting. Wasnt exacty sure which one is better. Hence put both.
		*
		pid_t child_pid[currNode->num_children];
		for(int i = 0; i < currNode->num_children; i++) {
			if (getNode(nodes, len, currNode->children[i])->status == 0) {
				child_pid[i] = fork();
				if (child_pid[i] == 0) {
					DFSVisit(nodes, len, getNode(nodes, len, currNode->children[i]), numCand, candString);
					return;
				} else {
					wait(NULL);
					execNodes(currNode, numCand, candString);
				}
			}
		}
		for(int i = 0; i < currNode->num_children; i++) {
			wait(NULL);
		}
		*/
	}
	currNode->status = 2; //convert the grey node to a black node
}


void DFS(struct node * nodes, int len, char * numCand, char * candString) {
	// initialize graph
	initializeGraph(nodes, len);
	for(int i = 0; i < len; i++) {
		if (nodes[i].status == 0) {
			DFSVisit(nodes, len, &nodes[i], numCand, candString);
		}
	}
}




/**Function : parseInput
 * Arguments: 'filename' - name of the input file
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: Number of Total Allocated Nodes
 * About parseInput: parseInput is supposed to
 * 1) Open the Input File [There is a utility function provided in utility handbook]
 * 2) Read it line by line : Ignore the empty lines [There is a utility function provided in utility handbook]
 * 3) Call parseInputLine(..) on each one of these lines
 ..After all lines are parsed(and the DAG created)
 4) Assign node->"prog" ie, the commands that each of the nodes has to execute
 For Leaf Nodes: ./leafcounter <arguments> is the command to be executed.
 Please refer to the utility handbook for more details.
 For Non-Leaf Nodes, that are not the root node(ie, the node which declares the winner):
 ./aggregate_votes <arguments> is the application to be executed. [Refer utility handbook]
 For the Node which declares the winner:
 This gets run only once, after all other nodes are done executing
 It uses: ./find_winner <arguments> [Refer utility handbook]
 */
int parseInput(char *filename, node_t *n) {
	if(access( filename, F_OK ) == -1 ) {	//check if file exists or not.
		printf("Input file - %s not found. Exiting the program.\n", filename);
		exit(-1);
	} 
	FILE* f = fopen(filename, "r"); 
	//opening the file
	char* buf = (char *) malloc(1024 * sizeof(char)); //allocate memory
	int x = 1024;

	int flag = 0;
	while (!flag && getline(&buf, &x, f)  != EOF) {	//get the first line while ignoring the lines starting with #
		if (buf[0] != '#') {
			flag = 1;
		}
	}
	if (flag == 0) {
		printf("%s\n", "The file does not contain enough lines.");
		exit(-1);
	}

	// function calls below help extract data from the first line.
	int noOfCandidates = getNumOfCandidates(buf);

	char * numCand = getStringNumOfCandidates(buf);

	char * candString = malloc(1024 * sizeof(char)); //allocate memory
	strcpy(candString, getStringOfCandidates(buf));

	flag = 0;
	while (!flag && getline(&buf, &x, f)  != EOF) {	//get the second line while ignoring the lines starting with #
		if (buf[0] != '#') {
			flag = 1;
		}
	}
	if (flag == 0) { //error handling
		printf("%s\n", "The file does not contain enough lines.");
		exit(-1);
	}

	// error handling specially for considering whether there is a Who_Won node.
	int status = checkValidNodes(buf);
	if (status == -1) {
		printf("%s\n", "The nodes does not contain Who_Won Node. Please edit the input file.");
		exit(-1);
	}

	// function calls below help extract data from the second line.
	int noOfNodes = getNoOfNodes(buf);
	struct node* nodeList = getAllNodes(buf);
	int counterOfLines = 0;

	while(getline(&buf, &x, f)  != EOF) {
		if (buf[0] == '#') {
			// do nothing - Commented out line
		} else {
			counterOfLines += 1;
			addNeighbors(nodeList, noOfNodes, buf);
		}
	}

	if (counterOfLines == 0) { //error handling
		printf("%s\n", "The file has not provided the necessary lines to create a DAG. Either they are commented out # or do not exist.");
		exit(-1);
	}

	// function calls to actually add information about executable and input files in all the nodes in DAG.
	addProg(nodeList, noOfNodes);
	addInputs(nodeList, noOfNodes);

	// error handling.
	checkForWhoWonHasNeighbors(nodeList, noOfNodes);

	//***************************************************************************//
	// Segment of code below prints the DAG //
	//***************************************************************************//
	for(int i = 0; i < noOfNodes; i++) {
		if (nodeList[i].num_children == 0) {
			printf("%s(%s, %s, %s) --> ", nodeList[i].name, nodeList[i].output, nodeList[i].input[0], nodeList[i].prog);
		} else {
			printf("%s(%s, %s) --> ", nodeList[i].name, nodeList[i].output, nodeList[i].prog);
		}
		for(int j = 0; j < nodeList[i].num_children; j++) {
			printf("%s(%d, %s) ", getName(nodeList, noOfNodes, nodeList[i].children[j]), nodeList[i].children[j], nodeList[i].input[j]);
		}
		printf("\n");
		printf("\n");
	}
	//***************************************************************************//
	//***************************************************************************//
	int check = 0;
	backCycleCheck(nodeList, noOfNodes, numCand, candString, &check);	// check whether back cycle exists.
	if (check == -1) {
		return -1;
	} else {
		DFS(nodeList, noOfNodes, numCand, candString);
	}
	return 0;
}




int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES); //allocate memory

	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	//call parseInput
	int num = parseInput(argv[1], mainnodes);


	//Call execNodes on the root node


	return 0;
}
