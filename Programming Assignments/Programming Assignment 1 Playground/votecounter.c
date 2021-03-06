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


//***************************************************************************//
// Get Candidate Information from the Line 1 Using these functions below //
//***************************************************************************//

int getNumOfCandidates(char *buf) {
	char ** argvp = NULL;
	if (makeargv(trimwhitespace(buf), " ", &argvp) == 0) {
		printf("%s\n", "The first line does not contain data in the correct format. Please make sure it is correct.");
		exit(-1);
	} else {
		int num = atoi(argvp[0]);
		if (num == 0) {
			printf("%s\n", "The first line does not contain a number at starting. Please change the input file.");
			exit(-1);
		}
		return num;
	}
}

char * getStringNumOfCandidates(char *line) {
	char ** argvp = NULL;
	if (makeargv(trimwhitespace(line), " ", &argvp) == 0) {
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
	int len = makeargv(trimwhitespace(line), " ", &argvp);
	if ( len == 0) {
		printf("%s\n", "The first line does not contain data in the correct format. Please make sure it is correct.");
		exit(-1);
	} else {
		if (len != atoi(argvp[0]) + 1) {
			printf("%s\n", "The number of candidates given does not satisfy the number of candidate names specified in the input file.");
			exit(-1);
		} else {
			for (int i = 1; i < len; i++) {
				for (int j = i+1; j < len; j++) {
					if (strcmp(argvp[i], argvp[j]) == 0) {
						printf("%s\n", "The candidates have duplicate names. Currently the system does not support having duplicates candidate names.");
						exit(-1);
					}
				}
			}
			char * candstring = (char *) malloc(sizeof(char) * 1024);
			strcpy(candstring, "");
			for (int i = 1; i < len;  i++) {
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
	int len = makeargv(trimwhitespace(line), " ", &argvp);

	if (len == 0) {
		printf("%s\n", "The number of nodes given equal to zero. Please edit the input file.");
		exit(-1);
	} else {
		for (int i = 0; i < len; i++) {
			for (int j = i+1; j < len; j++) {
				if (strcmp(argvp[i], argvp[j]) == 0) {
					printf("%s\n", "The nodes have duplicates. Currently the system does not support having duplicates node.");
					exit(-1);
				}
			}
		}
		for (int i = 0; i < len; i++) {
			if (strcmp(argvp[i], "Who_Won") == 0) {
				return 0;
			}
		}
		return -1;
	}
}

int getNoOfNodes(char* line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp);
	printf("Total number of Nodes in the DAG = %d\n\n", len);
	if (len < 2) {
		printf("The number of nodes given equal to %d. Should atleast have 2 or more nodes specified. Please edit the input file.\n", len);
		exit(-1);
	}
	return len;
}

struct node* getAllNodes(char* line) {
	char ** argvp = NULL;
	int len = makeargv(trimwhitespace(line), " ", &argvp);

	struct node* nodes = (struct node *)malloc(len * sizeof(struct node)); //allocate memory

	if (len == 0) {
		printf("%s\n", "The number of nodes given equal to zero. Please edit the input file.");
		exit(-1);
	} else {
		for(int i = 0; i < len; i++) {
			char* str1 = "Output_";
			nodes[i] = (struct node){"", "", "", "", NULL, 0, 0, 0, 0};
			strcpy(nodes[i].name, argvp[i]);
			strcpy(nodes[i].output, argvp[i]);
			nodes[i].pid = i;
			nodes[i].id = i;
			prepend(nodes[i].output,str1); 
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
		if (strcmp(nodes[i].name, name) == 0) {
			return nodes[i].id;
		}
	}
	return -1;
}

char * getName(struct node* nodes, int len, int id) {
	for (int i = 0; i < len; i++) {
		if (nodes[i].id == id) {
			return nodes[i].name;
		}
	}
	return "<NotFound>";
}

struct node * getNode(struct node* nodes, int len, int id) {
	for (int i = 0; i < len; i++) {
		if (nodes[i].id == id) {
			return &nodes[i];
		}
	}
	return NULL;
}

int checkValidNodeName(struct node* nodes, int len, char nodename[]) {
	for (int i = 0; i < len; i++) {
		if (strcmp(nodes[i].name, nodename) == 0) {
			return 0;
		}
	}
	return -1;
}
//***************************************************************************//
//***************************************************************************//



//***************************************************************************//
// Functions below help create the DAG using the functions above as helpers//
//***************************************************************************//

void addNeighbor(struct node* nodes, int len, char parent[], char child[]) {
	if (checkValidNodeName(nodes, len, parent) == -1) {
		printf("%s %s\n", "The name of the parent is not valid. Not allowed. Parent-", parent);
		exit(-1);
		return;
	}
	for (int i = 0; i < len; i++) {
		if (strcmp(nodes[i].name, parent) == 0) {
			int id = getID(nodes, len, child);
			if (id == -1) {
				printf("%s %s\n", "The name of the child node is not valid (child will be ommited). Child-", child);
				return;
			}
			nodes[i].children[nodes[i].num_children] = id;
			nodes[i].num_children += 1;
			return;
		}
	}
}

int addNeighbors(struct node* nodes, int len, char* line) {
	char ** argvp = NULL;
	int temp = makeargv(trimwhitespace(line), ":", &argvp);
	if (temp < 2) {
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
	for (int i = 0; i < noOfNeighbors; i++) {
		addNeighbor(nodes, len, parent, trimwhitespace(arg[i]));
	}
	return 0;
}

void addProg(struct node* nodes, int len) {
	int flag = 0;
	for (int i = 0; i < len; i++) {
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

void addInput(struct node* nodes, int len, struct node * currNode) {
	if (currNode->num_children == 0) {
		strcpy(currNode->input[0], currNode->name);
	} else {
		for (int i = 0; i < currNode->num_children; i++) {
			strcpy(currNode->input[i], getNode(nodes, len, currNode->children[i])->output);
		}
	}
}

void addInputs(struct node* nodes, int len) {
	for (int i = 0; i < len; i++) {
		addInput(nodes, len, &nodes[i]);
	}
}
//***************************************************************************//
//***************************************************************************//



void checkForWhoWonHasNeighbors(struct node* nodes, int len) {
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
 * Arguments: 'n' - Pointer to Nodes to be allocated by parsing
 * About execNodes: parseInputLine is supposed to
 * If the node passed has children, fork and execute them first
 * Please note that processes which are independent of each other
 * can and should be running in a parallel fashion
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
    if (chmod ("leafcounter",i) < 0)  {
			printf("%s\n", "chmod command on leafcounter didnt work. Exiting the program.");
			exit(-1);
		}
		if(access( args[1], F_OK ) != -1 ) {
		  // file exists
		} else {
			printf("Input file - %s not found. Exiting the program.\n", args[1]);
			exit(-1);
		} 
		if (chmod (args[1],i) < 0)  {
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
			if(access( args[j+2], F_OK ) == -1 ) {
				printf("Input file - %s not found. Exiting the program.\n", args[j+2]);
				exit(-1);
			} 
			if (chmod (args[j+2],i) < 0)  {
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


    if (chmod ("aggregate_votes",i) < 0)  {
			printf("%s\n", "chmod command on aggregate_votes didnt work. Exiting the program.");
			exit(-1);
		}
		if (chmod ("find_winner",i) < 0)  {
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
			//
		} else {
			wait(NULL);
			execNodes(currNode, numCand, candString);
		}
	} else {
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
		/* Wait for children to exit. */
		for(int i = 0; i < currNode->num_children; i++) {
			wait(NULL);
		}
	}
	// 	if (child_pid == 0) {
	// 		for(int i = 0; i < currNode->num_children; i++) {
	// 			if (getNode(nodes, len, currNode->children[i])->status == 0) {
	// 				DFSVisit(nodes, len, getNode(nodes, len, currNode->children[i]), numCand, candString);
	// 			}
	// 		}
	// 	} else {
	// 		wait(NULL);
	// 		execNodes(currNode, numCand, candString);
	// 	}
	// }
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
	if(access( filename, F_OK ) == -1 ) {
		printf("Input file - %s not found. Exiting the program.\n", filename);
		exit(-1);
	} 
	FILE* f = fopen(filename, "r"); 
	//opening the file
	char* buf = (char *) malloc(1024 * sizeof(char));
	int x = 1024;

	int flag = 0;
	while (!flag && getline(&buf, &x, f)  != EOF) {
		if (buf[0] != '#') {
			flag = 1;
		}
	}
	if (flag == 0) {
		printf("%s\n", "The file does not contain enough lines.");
		exit(-1);
	}

	// function to load the first line in the char *buffer.
	int noOfCandidates = getNumOfCandidates(buf);

	// struct candidate* candidates = getCandidates(buf);
	char * numCand = getStringNumOfCandidates(buf);

	char * candString = malloc(1024 * sizeof(char));
	strcpy(candString, getStringOfCandidates(buf));

	flag = 0;
	while (!flag && getline(&buf, &x, f)  != EOF) {
		if (buf[0] != '#') {
			flag = 1;
		}
	}
	if (flag == 0) {
		printf("%s\n", "The file does not contain enough lines.");
		exit(-1);
	}

	int status = checkValidNodes(buf);
	if (status == -1) {
		printf("%s\n", "The nodes does not contain Who_Won Node. Please edit the input file.");
		exit(-1);
	}

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

	if (counterOfLines == 0) {
		printf("%s\n", "The file has not provided the necessary lines to create a DAG. Either they are commented out # or do not exist.");
		exit(-1);
	}

	addProg(nodeList, noOfNodes);
	addInputs(nodeList, noOfNodes);

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
	backCycleCheck(nodeList, noOfNodes, numCand, candString, &check);
	if (check == -1) {
		return -1;
	} else {
		DFS(nodeList, noOfNodes, numCand, candString);
	}
	return 0;
}




int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	//call parseInput
	int num = parseInput(argv[1], mainnodes);


	//Call execNodes on the root node


	return 0;
}
