#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

struct files_to_scan { //With this struct, we will create the nodes of our binomial heap

    char file_name[50]; //This string variable holds the name of our file in the struct
    int relevancy_of_words;//This integer variable holds the relevancy of the keyword in the struct
    int degree_of_node;//This integer variable holds the degree of the node of binomial heap

    struct files_to_scan *parent; //these 3 nodes are necessary for creating the binomial heap
    struct files_to_scan *sibling;
    struct files_to_scan *child;
};

typedef struct files_to_scan files_to_scan; //we typedef the struct to use it easily

files_to_scan *global_header_1,*global_header_2; //we will use these headers in more then one function so i define them as global

void edit_func_for_debug_delete_function(files_to_scan *binomial_heap); //this is a prototype for one of our functions


files_to_scan *create_file_nodes_to_add_heap(char *file_name, int relevancy_of_keyword) { //after we scan the files, we will create nodes with their info by using this function
    files_to_scan *new_scanned_file; //first of all we create a new files to scan node
    new_scanned_file = malloc(sizeof(files_to_scan)); //we create a memory space for this new node with malloc function
    strcpy(new_scanned_file->file_name, file_name); //we assign its file name by using strcpy function
    new_scanned_file->relevancy_of_words = relevancy_of_keyword; //we can directly assign other values
    new_scanned_file->degree_of_node = 0;
    new_scanned_file->child=NULL;
    new_scanned_file->sibling=NULL;
    new_scanned_file->parent=NULL;
    return new_scanned_file; //this function return the node which have the info of our scanned file
}

files_to_scan *merge_bin_heaps(files_to_scan *heap_to_merge_1, files_to_scan *heap_to_merge_2) { //this function merges 2 binomial heaps. This is one of the necessary operations for creating a binomial heap
    files_to_scan *previous_node=NULL; //we need these 4 nodes to merge operation
    files_to_scan *next_node=NULL;
    files_to_scan *current_node=NULL;
    files_to_scan *template_node=NULL;

    if ((heap_to_merge_1 != NULL && heap_to_merge_2 != NULL && heap_to_merge_1->degree_of_node > heap_to_merge_2->degree_of_node)
        || heap_to_merge_1==NULL)
        global_header_1 = heap_to_merge_2; //we assign our first global header as the second heap that we want to merge whether first heap is null or first heap's degree is greater then the second heap's degree.
    else
        global_header_1 = heap_to_merge_1; //otherwise we assign our first global header as first heap that we want to merge


    while (heap_to_merge_1 && heap_to_merge_2) { //for doing below merging operations, both of the heaps must not be null
        if (heap_to_merge_2->degree_of_node == heap_to_merge_1->degree_of_node) { //if the degrees of both heaps are equal, we should link second heap as first heap's sibling
            template_node = heap_to_merge_1->sibling; //we hold our first heap's sibling in a template node to dont lose it
            heap_to_merge_1->sibling = heap_to_merge_2; //then we link our second heap as sibling of our first heap
            heap_to_merge_1 = template_node; //then we assign our first heap as template heap so we are able to scan it correctly again
        }
        else if (heap_to_merge_2->degree_of_node < heap_to_merge_1->degree_of_node) { //if the degree of first heap is greater than the degree of second heap, we should link first heap as second heap's sibling
            template_node = heap_to_merge_2->sibling;  //we hold our second heap's sibling in a template node to dont lose it
            heap_to_merge_2->sibling = heap_to_merge_1; //then we link our first heap as sibling of our second heap
            heap_to_merge_2 = template_node; //then we assign our second heap as template heap so we are able to scan it correctly again

        }
        else
            heap_to_merge_1 = heap_to_merge_1->sibling; //if the degree of second heap is greater than the degree of first heap, we directly continue to scanning our first
    }

    if (global_header_1 == NULL){ //to do the remaining necessary operations, global_header_1 must been assigned before
        printf("Error occured while merging heaps");
        return 0;
    }
    else {
        previous_node = global_header_1->parent; //we assign previous,current and next nodes related to global_header_1
        current_node = global_header_1;
        next_node = global_header_1->sibling;

        while (next_node) {
            if ((current_node->degree_of_node != next_node->degree_of_node) ||((next_node->sibling != NULL) && next_node->degree_of_node-1 == current_node->degree_of_node)) {
                previous_node = current_node; //if whether the degrees of current and next nodes are not equal OR next node has not a sibling AND next node's sibling's degree is equal to current node's degree
                current_node = next_node; //we assign previous_node as current_node and current_node as next_node then we continue to scan
            }
            else if(current_node->relevancy_of_words <= next_node->relevancy_of_words) { //if the above condition is not true, then we check whether next node's relevancy_of_words value is greater then current's or not
                current_node->sibling = next_node->sibling; //if it is true we link next node's sibling as current node's sibling,
                next_node->parent = current_node;//current node as next node's parent,
                next_node->sibling = current_node->child;//next node's sibling as current node's child,
                current_node->child = next_node; //next node as current node's child'
                current_node->degree_of_node++; //and at the end we increase the degree of current node
            }
            else if(previous_node == NULL)  { //if both of above conditions are not true, then we check whether previous_node is null or not,
                global_header_1 = next_node; //if it is true, then we make global_header_1 our next node first.
                current_node->parent = next_node; //then we link next_node as current node's parent
                current_node->sibling = next_node->child; //then we link next_node's child as current node's sibling
                next_node->child = current_node; //then we link current node as next node's child
                next_node->degree_of_node++; //then we increase the degree of next node
                current_node = next_node; //at the end we assign next node as our current node and we keep scanning
            }
            else { //if all other conditions above are false,
                previous_node->sibling = next_node; //then we link next node as previous node's sibling'
                current_node->parent = next_node; //also we link next node as current node's parent
                current_node->sibling = next_node->child; //we link next node's child as current node's sibling
                next_node->child = current_node; //we link current node as next node's child
                next_node->degree_of_node++; //we increase the degree of next node
                current_node = next_node; //at the end we assign current node as next node and we keep scanning
            }

            next_node = current_node->sibling;//before continuing the loop we make current node's sibling our next node
        }
    }
    return global_header_1; // after all of these operations, our global header 1 will be the united heap.
}

files_to_scan *insert_new_nodes_to_heap(files_to_scan *header_node, files_to_scan *new_node_to_add) {
    header_node = merge_bin_heaps(header_node, new_node_to_add); //our merge_bin_heaps function allows us to insert a single node to our tree
    return header_node;
}

void spin_the_heap(files_to_scan *binomial_heap) { //we need this spin function to use it in delete function
    if (binomial_heap->sibling != NULL) {
        spin_the_heap(binomial_heap->sibling); //by calling spin function recursively we reach the end of the heap
        binomial_heap->sibling->sibling = binomial_heap; //we spin the links here
    } else
        global_header_2 = binomial_heap; //we spin the root here
}


files_to_scan *delete_the_node_with_min_key(files_to_scan *binomial_heap) { //this is our dequeue function. it finds and deletes the node that have min relevancy value from the heap.
    files_to_scan *node_with_min_key = binomial_heap; //we check all nodes oen by one so at the beginning we assume header is the min node
    files_to_scan *template_node_1 = NULL; //we need 2 node templates for following swap operations
    files_to_scan *template_node_2 = binomial_heap;
    files_to_scan *current_node = binomial_heap; //this heap represents the current node that we are checking so at the beginning it is header

    global_header_2 = NULL; //we will use our second global header in this function so it should be empty at the beginning

    if (current_node == NULL) { //if current node is null that means binomial heap doesnt have any node to delete
        printf("Error occurred. The heap is empty");
        return current_node;
    }

    while (template_node_2->sibling) { //while our second template node has a sibling,
        if ((template_node_2->sibling)->relevancy_of_words < node_with_min_key->relevancy_of_words) { //we check whether the sibling of the second template node is smaller then the current min node
            node_with_min_key = template_node_2->sibling;//if it is smaller then we assign sibling of second template node as min node.
            template_node_1 = template_node_2; //we assign second template node's value to first one to dont lose it
        }
        template_node_2 = template_node_2->sibling;//we update second template node to keep checking
    }

    //we found the node with the min and we hold it in node_with_min_key now. With the operations below we delete it from heap

    if (template_node_1 == NULL) {//if our first template node is null
        if(node_with_min_key->sibling == NULL){ //we check whether the sibling of current node is null or not
            binomial_heap = NULL;//if it is we assign binomial heap as null and we assign global header 2 as global header 1
            global_header_1 = global_header_2;
        }
        else {
            binomial_heap = node_with_min_key->sibling; //if current node has a sibling, we assign it as binomial heap header
            global_header_1 = binomial_heap; //then we assign binomial heap header as global_header_1
            edit_func_for_debug_delete_function(global_header_2); //by using edit function we delete the node correctly
        }
    }
    else if (template_node_1->sibling == NULL) //if our first template node is not null but its sibling is null then we make template node 1 null too
        template_node_1 = NULL;
    else
        template_node_1->sibling = node_with_min_key->sibling; //if all other conditions above are not true then we made current node's sibling template node 1's sibling.

    edit_func_for_debug_delete_function(global_header_2); //then we call edit function again to correct deletion
    if (node_with_min_key->child != NULL) { //here if current node's child is not null then we spin the heap below current node's child
        spin_the_heap(node_with_min_key->child);
        (node_with_min_key->child)->sibling = NULL;
    }

    edit_func_for_debug_delete_function(global_header_2);//here we call the edit function last time and we complete the deleting operation
    return node_with_min_key; //at the end this current node is the node with min key which has been deleted from heap
}
void edit_func_for_debug_delete_function(files_to_scan *binomial_heap){ //if we try to delete a node directly, we would lose the child nodes of that node. With this function i debug this problem
    files_to_scan *current_node;
    //this function will add the child nodes of the node that we want to delete to heap one by one by correctly.
    while (binomial_heap) //binomial heap must not be null
    {
        current_node = create_file_nodes_to_add_heap("\0", 0); //we create a current node template with random values
        current_node->relevancy_of_words = binomial_heap->relevancy_of_words; //we assign the values of nodes one by one to current node
        sprintf(current_node->file_name, "%s", binomial_heap->file_name);
        global_header_1 = insert_new_nodes_to_heap(global_header_1, current_node);//we insert our current node to heap one by one
        edit_func_for_debug_delete_function(binomial_heap->child);//by calling this function recursively we do the operations above for all nodes one by one
        binomial_heap = binomial_heap->sibling;//we update the value here at every iteration
    }
}
int calculate_the_file_count(DIR *directory, struct dirent *dir, int file_count){ //with this function we calculate that how many files is there in a directory
    if (directory){

        while((dir = readdir(directory))){
            char filename[1000]; // Make sure this is large enough.
            sprintf(filename, "%s\\%s", "files", dir->d_name);

            if(strcmp(filename, "files\\.")!=0 && strcmp(filename, "files\\..")!=0)
                file_count++; //this file count increase for each file (except some files with name "." or "..")
        }
    }
    return file_count;
}


files_to_scan find_5_files_with_maximun_relevancy(files_to_scan maximun_files_array[5]) {
    int i;
    printf("The relevance order is: ");
    char fileName[1000] = "";
    //delete function normally returns the node with minimun key. To use this function more effectively, i extract the relevancy scores from a huge number.
    //lets assume maximun relevancy is 15. When i extract it from huge number it will be the minimun.
    //So when we call delete function it returns the node with the maximun key by this implementation
    for (i = 0; i < 5; i++) {
        //we call this function for 5 times to find 5 max nodes
        files_to_scan *node_with_minimun_key = delete_the_node_with_min_key(global_header_1);

        if (9999 - node_with_minimun_key->relevancy_of_words != 0) {//we ignore the files with 0 relevancy
            sprintf(fileName, node_with_minimun_key->file_name);
            FILE *filePtr = fopen(fileName, "r");

            if (filePtr == NULL) {
                printf("%s, Cannot open file!", fileName);
                exit(0);
            }
            maximun_files_array[i]=*node_with_minimun_key;
            //IMPORTANT: THESE ARE CASE SENSITIVE SCORES. For an example Anybody and anybody are different.
            if(i!=4)
                printf("%s (%d), ", maximun_files_array[i].file_name, 9999 - maximun_files_array[i].relevancy_of_words);
            else
                printf("%s (%d). ", maximun_files_array[i].file_name, 9999 - maximun_files_array[i].relevancy_of_words);
            fclose(filePtr);
        }
    }
}

void print_files(FILE *file_to_print,files_to_scan maximun_files_array[5],int files_with_zero_relevancy,int file_count){
    int i; //this function prints the files with maximun relevancy scores
    for(i=0;i<5;i++){
        if(i<file_count-files_with_zero_relevancy){
            file_to_print = fopen(maximun_files_array[i].file_name,"r");
            char c;
            printf("%s(%d): ",maximun_files_array[i].file_name,9999-maximun_files_array[i].relevancy_of_words);
            while((c=fgetc(file_to_print))!=EOF)
                putchar(c);
            printf("\n\n");
        }}

}

int main() {
    char keyword[50]; //we will hold the keyword that we get from user in thischar array
        files_to_scan* template_node;
        int files_with_zero_relevancy=0;
        int file_count = 0;

        printf("Please enter a keyword to scan: ");
        scanf("%s", &keyword);
        DIR *directory;
        struct dirent *dir;

        directory = opendir("files"); //we open our directory here
        int i = 0 , j = 0 , t; //we will use these variables
        file_count=calculate_the_file_count(directory, dir, file_count); //we calculate the file count
        closedir(directory);
        directory=opendir("files"); //we open our directory again for calculate relevancy scores
        files_to_scan scanned_files[file_count];//we will hold our files in this array
        if (directory)
        {
            while ((dir = readdir(directory))) {
                char filename[1000]; // Make sure this is large enough.
                sprintf(filename, "%s\\%s", "files", dir->d_name);
                if(strcmp(filename, "files\\.")!=0 && strcmp(filename, "files\\..")!=0)
                {
                    strcpy(scanned_files[i].file_name , filename);
                    FILE* fin = fopen(filename, "r");//we open our txt file here

                    if ( fin != NULL )
                    {
                        int count=0;
                        char cArray[50];
                        char c;

                        while((c = fgetc(fin)) != EOF)
                        {
                            if(c == ' ' || c == '\n' || c == ':' || c == '.' || c == ',' || c == '-' || c == '(' || c == ')')
                            {
                                //we read txt files char by char here and when we gain a complete word, we compare it with the keyword

                                if(strcmp(keyword,cArray)==0)
                                    count++;//if they are equal we increase count
                                for(t=0;t<50;t++) {
                                    cArray[t] = '\0';//then we clear our char array
                                }
                                j = 0;
                                continue;
                            }
                            cArray[j]=c;
                            j++;
                        }
                        if(count==0)
                            files_with_zero_relevancy++;
                        scanned_files[i].relevancy_of_words=count;
                        //printf("%s , %d\n", scanned_files[i].file_name,scanned_files[i].relevancy_of_words);
                        //here we create our heap with our functions
                        template_node = create_file_nodes_to_add_heap(scanned_files[i].file_name, 9999 - scanned_files[i].relevancy_of_words);
                        global_header_1 = insert_new_nodes_to_heap(global_header_1, template_node);
                        i++;

                        fclose(fin);
                    }
                }
            }
            closedir(directory);
        }
        files_to_scan maximun_files_array[5];
        FILE *fin;
        //we print our output with these functions that we defined above
        find_5_files_with_maximun_relevancy(maximun_files_array);
        printf("\n\n\nThere are %d files with 0 relevancy score.\n\n",files_with_zero_relevancy);
        print_files(fin,maximun_files_array,files_with_zero_relevancy,file_count);
    return 0;
}
