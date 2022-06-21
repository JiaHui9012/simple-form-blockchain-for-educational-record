#include<cstdio>
#include<ctime>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

//group educational record information into a data type
struct Data {
    int completion_date; //completion date
    int student_id; //student ID
    string student_name; //student name
    string level; //education level
    string major; //student's major
    string grades; //student's grade
};

//group all the elements inside a block into a data type
struct Block{
    int index; //block index
    int blockHash; //block hash
    int previousHash; //previous block hash
    int nextHash; //next block hash
    string timestamp; //timestamp
    struct Data data; //Information
    struct Block *prevBlock; //previous block
    struct Block *nextBlock; //next block
}*head;

//a function to generate a hash for a block
int generateHash(Data d){
    hash<string> hash1;
    hash<int> hash2;
    hash<int> hash3;
    int intHash = d.completion_date + d.student_id;
    string strHash = d.student_name + d.level + d.major + d.grades;
    int hash4 = hash3(hash1(strHash) + hash2(intHash));
    int finalHash = abs(hash4);

    return finalHash;
}

//a function to count how many blocks are exist
int getCount(){
    int count = 0; //initialize count
    Block* current = head; //initialize current
    while (current != NULL){
        count++;
        current = current->nextBlock; //put the next block in
    }
    return count;
}

//a function to get the timestamp for a block
string getTimestamp(){
    time_t rawtime;
    tm* timeinfo;
    char buffer [80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y%m%d, %H:%M:%S",timeinfo);
    return buffer;
}

//a function to add new block with the parameter data
void addBlock(Data d){
    if(head == NULL){ //if head is null, create the head
        head = new Block();
        head->index = getCount();
        head->blockHash = generateHash(d);
        head->previousHash = head->blockHash;
        head->nextHash = head->blockHash;
        head->timestamp = getTimestamp();
        head->data = d;
        head->prevBlock = NULL;
        head->nextBlock = NULL;
        return;
    }
    Block *currentBlock = head; //initialize currentBlock
    while(currentBlock->nextBlock){ //get the last block using loop
        currentBlock = currentBlock->nextBlock;
    }
    Block *newBlock = new Block(); //create new block
    currentBlock->nextBlock = newBlock; //make the new block become the last block in the chain
    newBlock->index = (currentBlock->index) + 1;
    newBlock->blockHash = generateHash(d);
    currentBlock->nextHash = newBlock->blockHash;
    newBlock->previousHash = currentBlock->blockHash;
    newBlock->nextHash = newBlock->blockHash;
    newBlock->timestamp = getTimestamp();
    newBlock->data = d;
    newBlock->prevBlock = currentBlock;
    newBlock->nextBlock = NULL;
}

//a function to add existing and stored block with its parameters index, timestamp, and data
void addBlock(int i,string t,Data d){
    if(head==NULL){
        head=new Block();
        head->index=i;
        head->blockHash=generateHash(d);
        head->previousHash=head->blockHash;
        head->nextHash=head->blockHash;
        head->timestamp=t;
        head->data=d;
        head->prevBlock=NULL;
        head->nextBlock=NULL;
        return;
    }
    Block *currentBlock=head;
    while(currentBlock->nextBlock){
        currentBlock=currentBlock->nextBlock;
    }
    Block *newBlock=new Block();
    currentBlock->nextBlock=newBlock;
    newBlock->index=i;
    newBlock->blockHash=generateHash(d);
    currentBlock->nextHash=newBlock->blockHash;
    newBlock->previousHash=currentBlock->blockHash;
    newBlock->nextHash=newBlock->blockHash;
    newBlock->timestamp=t;
    newBlock->data=d;
    newBlock->prevBlock=currentBlock;
    newBlock->nextBlock=NULL;
}

//a function to verify the chain, using the pointer/array to store the verified message
int* verifyChain(){
    int array_size=getCount(); //initialize the array size by counting how many blocks exist
    int* verifyMsgArray=new int[array_size]; //create an array
    Block *curr=head->nextBlock,*prev=head;
    int count=0;
    if(prev->previousHash!=prev->blockHash){ //if it is the first block,check if its previous block hash is equal to its block hash
        verifyMsgArray[count]=1;
    }
    else{
        verifyMsgArray[count]=0;
    }
    while(curr){
        if(prev->nextHash!=curr->blockHash){ //check if the previous block's next block hash is equal to the current block's block hash
            verifyMsgArray[count] = verifyMsgArray[count] == 1? 3 : 2;
        }
        count++;
        if(curr->previousHash!=prev->blockHash){ //check if the current block's previous block hash is equal to the previous block's block hash
            verifyMsgArray[count] = 1;
        }
        else{
            verifyMsgArray[count]=0;
        }
        prev=curr;
        curr=curr->nextBlock;
    }
    if(prev->nextHash!=prev->blockHash){ //if it is the last block, check if its next block hash is equal to its block hash
        verifyMsgArray[count] = verifyMsgArray[count] == 1? 3 : 2;
    }
    return verifyMsgArray;
}

//a function to print a block
void printBlock(Block* b){
    cout << "======================================================================" << endl;
    cout << "|| Block no.: " << b->index << endl;
    cout << "|| Current Block Hash: " << b->blockHash << endl;
    cout << "|| Previous Block Hash: " << b->previousHash << endl;
    cout << "|| Next Block Hash: " << b->nextHash << endl;
    cout << "|| Timestamp: " << b->timestamp << endl;
    cout << "|| Completion Date: " << b->data.completion_date << endl;
    cout << "|| Student ID: " << b->data.student_id << endl;
    cout << "|| Student Name: " << b->data.student_name << endl;
    cout << "|| Education Level: " << b->data.level << endl;
    cout << "|| Major: " << b->data.major << endl;
    cout << "|| Grades: " << b->data.grades << endl;
    cout << "======================================================================" << endl;
}

//a function to store all the block indexs into an array/pointer
int* getIndex(){
    int array_size=getCount(); //initialize the array size by counting how many blocks exist
    int* IndexArray=new int[array_size]; //create an array
    Block *curr=head;
    int count=0;
    while(curr){
        IndexArray[count]=curr->index; //store the index
        count++;
        curr=curr->nextBlock;
    }

    return IndexArray;
}

//a function to display all blocks and the chain
void display(){

    Block *curr=head,*prev;
    int total=getCount(),sum=0;
    cout << endl << "------------------------------------------------Display Section------------------------------------------------" << endl <<endl;
    cout << " Current Existing Blocks: " << total << endl << endl;

    if(curr!=NULL){
        int* VerifyArray=verifyChain();
        int* indexArray=getIndex();
        int x=0;
        cout << " Displaying all blocks and Verifying the hash and chain...  " << endl << endl;
        cout << "------------------------------Block Chain------------------------------" << endl;
        cout << "\t\t\t\t  ||" << endl;
        while(curr){ //print all blocks
            printBlock(curr);
            if(VerifyArray[x]==0){ //check if the block is valid
                cout << " VERIFIED!\t\t\t  ||" << endl;
                cout << "\t\t\t\t  ||" << endl;
            }
            else{
                cout << " VERIFICATION FAILED!\t\t  ||" << endl << endl;
            }
            x++;
            prev=curr;
            curr=curr->nextBlock;
        }
        cout << "-------------------------End of the Block Chain-------------------------" << endl << endl;
        for(int i=0;i<total;i++){
            sum += VerifyArray[i];
        }
        if(sum==0){ //check if all blocks are valid and if the chain is valid, then print the messages
            if(total==1){ //if there is only one block
                cout << " Verified Successfully! Only ONE block exist, therefore, NO chain exist... Add more blocks for me please!" << endl;
            }
            else{
                cout << " Verified Successfully! The chain is EXIST and VALID! WOOHOO~" << endl;
                cout << " The FIRST block of the chain is Block " << head->index << " and the LAST block of the chain is Block " << prev->index << "." << endl;
            }
        }
        else{ //if the chain is not valid, show the reasons
            cout << " Verification Failed! The chain is NOT valid! This happens due to:" << endl;
            for(int i=0;i<total;i++){
                switch(VerifyArray[i]){
                    case 1 :
                        if(i==0){
                            cout << " Block " << indexArray[i] << "'s previous block hash doesn't match with Block " << indexArray[i] << "'s current block hash." << endl;
                        }
                        else{
                            cout << " Block " << indexArray[i] << "'s previous block hash doesn't match with Block " << indexArray[i-1] << "'s current block hash." << endl;
                        }
                        break;
                    case 2 :
                        if(i==total-1){
                            cout << " Block " << indexArray[i] << "'s next block hash doesn't match with Block " << indexArray[i] << "'s current block hash." << endl;
                        }
                        else{
                            cout << " Block " << indexArray[i] << "'s next block hash doesn't match with Block " << indexArray[i+1] << "'s current block hash." << endl;
                        }
                        break;
                    case 3 :
                        if(i==0){
                            cout << " Block " << indexArray[i] << "'s previous block hash doesn't match with Block " << indexArray[i] << "'s current block hash." << endl;
                            cout << " Block " << indexArray[i] << "'s next block hash doesn't match with Block " << indexArray[i+1] << "'s current block hash." << endl;
                        }
                        else if(i==total-1){
                            cout << " Block " << indexArray[i] << "'s previous block hash doesn't match with Block " << indexArray[i-1] << "'s current block hash." << endl;
                            cout << " Block " << indexArray[i] << "'s next block hash doesn't match with Block " << indexArray[i] << "'s current block hash." << endl;
                        }
                        else{
                            cout << " Block " << indexArray[i] << "'s previous block hash doesn't match with Block " << indexArray[i-1] << "'s current block hash." << endl;
                            cout << " Block " << indexArray[i] << "'s next block hash doesn't match with Block " << indexArray[i+1] << "'s current block hash." << endl;
                        }
                        break;
                    default :
                        break;
                }
            }
        }
    }
    cout << endl << "---------------------------------------------------------------------------------------------------------------" << endl;
}

//a function to verify if the block user selected exists by using the block index to check
bool verifyBlock(int block){
    int* indexArray=getIndex();
    if(block>0){
        for(int x=0;x<getCount();x++){
            if(block==indexArray[x]){
                return true;
            }
        }
        return false;
    }
    else{
        return false;
    }
}

//a function to modify a block
void modifyBlock(int block){
    Data newData;
    Block *curr=head,*prev=NULL,*next=head->nextBlock;
    while(curr){
        if(curr->index==block){ //if the block index matches with the number user entered
            cout << endl << " Modifying block " << block << "..." << endl << endl;
            cout << " Below is the current information of Block " << block << endl;
            printBlock(curr);
            cout << endl << " Please enter the new data..." << endl;
            cout << " Completion date(YYYYMMDD): ";
            cin >> newData.completion_date;
            while(!cin){
                cout << endl << " Integer only~~~" << endl << " Completion date(YYYYMMDD): ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newData.completion_date;
            }
            cout << " Student ID: ";
            cin >> newData.student_id;
            while(!cin){
                cout << endl << " Integer only~~~" << endl << " Student ID: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> newData.student_id;
            }
            cin.ignore(100,'\n');
            cout << " Student Name: "; getline (cin, newData.student_name);
            cout << " Educational Level: "; getline (cin, newData.level);
            cout << " Major: "; getline (cin, newData.major);
            cout << " Grades: "; getline (cin, newData.grades);

            curr->blockHash=generateHash(newData); //assign new hash to the block
            curr->data=newData; //assign new data
            curr->timestamp=getTimestamp(); //update new timestamp
            if(curr==head){ //is the block is head block
                curr->previousHash=curr->blockHash;
                if(next==NULL){ //if the block is the only one block
                    curr->nextHash=curr->blockHash;
                }
                else{
                    next->previousHash=curr->blockHash;
                }
            }
            else if(next==NULL){ //if the block is the last block
                curr->nextHash=curr->blockHash;
                prev->nextHash=curr->blockHash;
            }
            else{
                prev->nextHash=curr->blockHash;
                next->previousHash=curr->blockHash;
            }

            cout << endl << " Modified Block " << block << " Successfully!" << endl << endl;
            cout << " Below is the new information of Block " << block << endl;
            printBlock(curr); //print new information of the block
            break;
        }
        prev=curr;
        curr=curr->nextBlock;
        next=curr->nextBlock;
    }
}

//a function to delete a block
void deleteBlock(int block){
    Block *curr=head,*prev=NULL,*next=head->nextBlock;
    while(curr){
        if(curr->index==block){ //if the block index matches with the number user entered
            cout << endl << " Deleting block " << block << "..." << endl << endl;
            cout << " Below is the information of Block " << block << endl;
            printBlock(curr);
            if(curr==head){ //if current block is head
                free(curr); //free the block
                curr=next;
                if(next==NULL){ //if it is the only block
                    head=curr;
                }
                else{
                    curr->previousHash=curr->blockHash;
                    curr->prevBlock=prev;
                    head=curr;
                }
            }
            else if(next==NULL){ //if the current block is the last block
                free(curr);
                curr=next;
                prev->nextHash=prev->blockHash;
                prev->nextBlock=curr;
            }
            else{
                free(curr);
                curr=next;
                prev->nextHash=curr->blockHash;
                prev->nextBlock=curr;
                curr->previousHash=prev->blockHash;
                curr->prevBlock=prev;
            }

            cout << endl << " Deleted Block " << block << " Successfully!" << endl;

            break;
        }
        prev=curr;
        curr=curr->nextBlock;
        next=curr->nextBlock;
    }
}

//a function to write the blocks' index, timestamp and data into a file
void WriteFile(){
    ofstream outfile;
    outfile.open("data.txt");
    Block *curr=head;
    while(curr){
        outfile << curr->index << "|";
        outfile << curr->timestamp << "|";
        outfile << curr->data.completion_date << "|";
        outfile << curr->data.student_id << "|";
        outfile << curr->data.student_name << "|";
        outfile << curr->data.level << "|";
        outfile << curr->data.major << "|";
        outfile << curr->data.grades << endl;
        curr=curr->nextBlock;
    }
    outfile.close();
}

//a function to read the data from a file
void Readfile(){
    ifstream inFile;
    inFile.open("data.txt");
    string line;
    while(getline(inFile,line)){

        stringstream ss(line);
        Data data;
        string completionDate;
        string studentID;
        string index_s;
        int index;
        string timestamp;
        char separator = '|'; //separate the data using '|'

        getline(ss,index_s,separator);
        index=stoi(index_s); //convert string to int
        getline(ss,timestamp,separator);
        getline(ss,completionDate,separator);
        data.completion_date=stoi(completionDate);  //convert string to int
        getline(ss,studentID,separator);
        data.student_id=stoi(studentID);  //convert string to int
        getline(ss,data.student_name,separator);
        getline(ss,data.level,separator);
        getline(ss,data.major,separator);
        getline(ss,data.grades);
        addBlock(index,timestamp,data);
    }
    inFile.close();
}

//a menu function to let user to choose what to do
void menu(){
    int choice;
    cout << endl << "---------------MENU--------------------------------------------------------------------------------------------" << endl << endl;
    cout << " Do you want to ..." << endl;
    cout << " 1. Add block" << endl;
    cout << " 2. Modify block" << endl;
    cout << " 3. Delete block" << endl;
    cout << " 4. Display and Verify" << endl;
    cout << " 5. Quit" << endl;
    cout << " Choice: ";
    cin >> choice;

    while(!cin){ //if user enters invalid value
        cout << endl << " Integer please! 1~2~3~ Choose again~" << endl << endl << " Choice: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> choice;
    }

    cout << endl << "---------------------------------------------------------------------------------------------------------------" << endl;
    switch(choice){
        case 1:{
            Block *curr=head;
            Data data;
            int index=1;
            int* indexArray=getIndex();
            if(getCount()!=0){
                index=indexArray[getCount()-1]+1;
            }
            //user input: let users key in the educational record information
            cout << endl << " Adding Block " << index << "..." << endl << endl << " Please enter the information below:" << endl;
            cout << " Completion date(YYYYMMDD): ";
            cin >> data.completion_date;
            while(!cin){
                cout << endl << " Integer only~~~" << endl << " Completion date(YYYYMMDD): ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> data.completion_date;
            }
            cout << " Student ID: ";
            cin >> data.student_id;
            while(!cin){
                cout << endl << " Integer only~~~" << endl << " Student ID: ";
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                cin >> data.student_id;
            }
            cin.ignore(100,'\n');
            cout << " Student Name: "; getline (cin, data.student_name);
            cout << " Educational Level: "; getline (cin, data.level);
            cout << " Major: "; getline (cin, data.major);
            cout << " Grades: "; getline (cin, data.grades);

            addBlock(data); //add block with the new data
            WriteFile(); //write the latest information into the file
            cout << endl << " Added Block " << index << " Successfully!" << endl <<endl;
            cout << " Below is the block you just added. Satisfied? Yes? Good! No? Go, go, go modify~" << endl;
            Block *addedBlock = head; //initialize addedBlock
            while(addedBlock->nextBlock){ //get the added block using loop
                addedBlock = addedBlock->nextBlock;
            }
            printBlock(addedBlock); //print information of the added block
            cout << endl << " Remember to verify the chain by choosing 4 in the menu section :D" << endl;
            menu(); //show menu
            break;}
        case 2:{
            Block *curr=head;

            if(curr==NULL){ //if chain size = 0, show no block to delete
                cout << endl << " NO block to modify.. Add block first la.. Choose 1 in the menu section, OK?" << endl;
            }
            else{
                int block;
                cout << endl << " Which block would you like to modify?" << endl << " Block: "; //let users choose which block to delete
                cin >> block;
                //if users enter invalid integer, let users enter again
                while(!cin || !verifyBlock(block)){
                    if(!verifyBlock(block)){
                        cout << endl << " Modified Block " << block << " Unsuccessfully! Due to block does NOT exist. Aiyo, Look carefully for the block number la.." << endl << " Block: ";
                    }
                    else{
                        cout << endl << " Do you know what is an integer? I guess you dont know because what you just entered was not an integer! Please enter again.." << endl << " Block: ";
                    }
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cin >> block;
                }
                modifyBlock(block); //modify selected block
                cout << endl << " It is important to always verify the chain~ you can choose the function in the menu section~" << endl;
            }
            WriteFile(); //write new data into file
            menu(); //show menu
            break;
        }
        case 3:{
            Block *curr=head;

            if(curr==NULL){ //if chain size = 0, show no block to delete
                cout << endl << " NO block to delete.. Can you add block first, so I can delete block and you also.. Im just kidding haha" << endl;
            }
            else{
                int block;
                cout << endl << " Which block would you like to delete?" << endl << " Block: "; //let users choose which block to delete
                cin >> block;
                //if users enter invalid integer, let users enter again
                while(!cin || !verifyBlock(block)){
                    if(!verifyBlock(block)){
                        cout << endl << " Deleted Block " << block << " Unsuccessfully! The block you selected does NOT exist, how am I supposed to delete? Enter again.." << endl << " Block: ";
                    }
                    else{
                        cout << endl << " Hello? That was not an integer! Give you one more chance, please enter again.." << endl << " Block: ";
                    }
                    cin.clear();
                    cin.ignore(INT_MAX, '\n');
                    cin >> block;
                }
                deleteBlock(block); //delete block
                cout << endl << " It will be a good choice to verify the chain now~ There is a verify function in the menu section~" << endl;
            }
            WriteFile(); //write the new data into file
            menu(); //back to menu
            break;}
        case 4:
            display(); //display and verify
            menu(); //back to menu
            break;
        case 5:
            cout << endl << " Bye, hooman~" << endl;
            exit(0); //quit/exit the program
        default:
            cout << endl << " Hey! Bad Choice! Choose again please~" << endl;
            menu(); //back to menu
            break;
    }
}

//main function
int main(){

    //the text file (data.txt) i provided is already included five data inside

    //read data from file first
    Readfile();
    //display the information
    display();
    //provide menu for user
    menu();

    return 0;
}
