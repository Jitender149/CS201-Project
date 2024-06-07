#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;
typedef struct merkle_tree_node{
    string hash;
    struct merkle_tree_node* left;
    struct merkle_tree_node* right;
}mtnode;
struct block{
    struct block* pnt;
    mtnode* Hash;
    mtnode* phash;
    //int trans_num;
    int block_no;
    string trans_data;
};

int b_no = 0;

typedef unsigned long long int int64;

int64 Message[80];

// Stores the hexadecimal values for
// calculating hash values
const int64 Constants[80]
	= { 0x428a2f98d728ae22, 0x7137449123ef65cd,
		0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
		0x3956c25bf348b538, 0x59f111f1b605d019,
		0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
		0xd807aa98a3030242, 0x12835b0145706fbe,
		0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
		0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
		0x9bdc06a725c71235, 0xc19bf174cf692694,
		0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
		0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
		0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
		0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
		0x983e5152ee66dfab, 0xa831c66d2db43210,
		0xb00327c898fb213f, 0xbf597fc7beef0ee4,
		0xc6e00bf33da88fc2, 0xd5a79147930aa725,
		0x06ca6351e003826f, 0x142929670a0e6e70,
		0x27b70a8546d22ffc, 0x2e1b21385c26c926,
		0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
		0x650a73548baf63de, 0x766a0abb3c77b2a8,
		0x81c2c92e47edaee6, 0x92722c851482353b,
		0xa2bfe8a14cf10364, 0xa81a664bbc423001,
		0xc24b8b70d0f89791, 0xc76c51a30654be30,
		0xd192e819d6ef5218, 0xd69906245565a910,
		0xf40e35855771202a, 0x106aa07032bbd1b8,
		0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
		0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
		0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
		0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
		0x748f82ee5defb2fc, 0x78a5636f43172f60,
		0x84c87814a1f0ab72, 0x8cc702081a6439ec,
		0x90befffa23631e28, 0xa4506cebde82bde9,
		0xbef9a3f7b2c67915, 0xc67178f2e372532b,
		0xca273eceea26619c, 0xd186b8c721c0c207,
		0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
		0x06f067aa72176fba, 0x0a637dc5a2c898a6,
		0x113f9804bef90dae, 0x1b710b35131c471b,
		0x28db77f523047d84, 0x32caab7b40c72493,
		0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
		0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
		0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };

// Function to convert a binary string
// to hexa-decimal value
string gethex(string bin)
{
	if (bin == "0000")
		return "0";
	if (bin == "0001")
		return "1";
	if (bin == "0010")
		return "2";
	if (bin == "0011")
		return "3";
	if (bin == "0100")
		return "4";
	if (bin == "0101")
		return "5";
	if (bin == "0110")
		return "6";
	if (bin == "0111")
		return "7";
	if (bin == "1000")
		return "8";
	if (bin == "1001")
		return "9";
	if (bin == "1010")
		return "a";
	if (bin == "1011")
		return "b";
	if (bin == "1100")
		return "c";
	if (bin == "1101")
		return "d";
	if (bin == "1110")
		return "e";
	if (bin == "1111")
		return "f";
}

// Function to convert a decimal value
// to hexa decimal value
string decimaltohex(int64 deci)
{
	// Stores the value as string
	string EQBIN = bitset<64>(deci).to_string();

	// Stores the equivalent hexa decimal
	string hexstring = "";
	string temp;

	// Traverse the string EQBIN
	for (unsigned int i = 0;
		i < EQBIN.length(); i += 4) {
		temp = EQBIN.substr(i, 4);
		hexstring += gethex(temp);
	}

	// Return the hexstring
	return hexstring;
}

// Function to convert a binary
// string to decimal value
int64 BintoDec(string bin)
{

	int64 value = bitset<64>(bin)
					.to_ullong();
	return value;
}

// Function to right rotate x by n bits
int64 rotate_right(int64 x, int n)
{
	return (x >> n) | (x << (64 - n));
}

// Function to right shift x by n bits
int64 shift_right(int64 x, int n)
{
	return (x >> n);
}

// Function to divide the string
// into chunks
void separator(string getBlock)
{
	// Stores the size of chunks
	int chunknum = 0;

	// Traverse the string S
	for (unsigned int i = 0;
		i < getBlock.length();
		i += 64, ++chunknum) {

		// Update the Message[chunknum]
		Message[chunknum]
			= BintoDec(getBlock.substr(i, 64));
	}

	// Iterate over the range [16, 80]
	for (int g = 16; g < 80; ++g) {

		// Find the WordA
		int64 WordA = rotate_right(Message[g - 2], 19)
					^ rotate_right(Message[g - 2], 61)
					^ shift_right(Message[g - 2], 6);

		// Find the WordB
		int64 WordB = Message[g - 7];

		// Find the WordC
		int64 WordC = rotate_right(Message[g - 15], 1)
					^ rotate_right(Message[g - 15], 8)
					^ shift_right(Message[g - 15], 7);

		// Find the WordD
		int64 WordD = Message[g - 16];

		// Find the resultant code
		int64 T = WordA + WordB + WordC + WordD;

		// Return the resultant Hash Code
		Message[g] = T;
	}
}

// Function to find the major of a, b, c
int64 maj(int64 a, int64 b, int64 c)
{
	return (a & b) ^ (b & c) ^ (c & a);
}

// Function to find the ch value of a,
// b, and c
int64 Ch(int64 e, int64 f, int64 g)
{
	return (e & f) ^ (~e & g);
}

// Function to find the Bitwise XOR with
// the right rotate over 14, 18, and 41
int64 sigmaE(int64 e)
{
	// Return the resultant value
	return rotate_right(e, 14)
		^ rotate_right(e, 18)
		^ rotate_right(e, 41);
}

// Function to find the Bitwise XOR with
// the right rotate over 28, 34, and 39
int64 sigmaA(int64 a)
{

	// Return the resultant value
	return rotate_right(a, 28)
		^ rotate_right(a, 34)
		^ rotate_right(a, 39);
}

// Function to generate the hash code
void Func(int64 a, int64 b, int64 c,
		int64& d, int64 e, int64 f,
		int64 g, int64& h, int K)
{
	// Find the Hash Code
	int64 T1 = h + Ch(e, f, g) + sigmaE(e) + Message[K]
			+ Constants[K];
	int64 T2 = sigmaA(a) + maj(a, b, c);

	d = d + T1;
	h = T1 + T2;
}

// Function to convert the hash value
// of a given string
string SHA512(string myString)
{
	// Stores the 8 blocks of size 64
	int64 A = 0x6a09e667f3bcc908;
	int64 B = 0xbb67ae8584caa73b;
	int64 C = 0x3c6ef372fe94f82b;
	int64 D = 0xa54ff53a5f1d36f1;
	int64 E = 0x510e527fade682d1;
	int64 F = 0x9b05688c2b3e6c1f;
	int64 G = 0x1f83d9abfb41bd6b;
	int64 H = 0x5be0cd19137e2179;

	int64 AA, BB, CC, DD, EE, FF, GG, HH;

	stringstream fixedstream;

	// Traverse the string S
	for (int i = 0;
		i < myString.size(); ++i) {

		// Add the character to stream
		fixedstream << bitset<8>(myString[i]);
	}

	// Stores string of size 1024
	string s1024;

	// Stores the string in the
	// fixedstream
	s1024 = fixedstream.str();

	// Stores the length of string
	int orilen = s1024.length();
	int tobeadded;

	// Find modded string length
	int modded = s1024.length() % 1024;

	// If 1024-128 is greater than modded
	if (1024 - modded >= 128) {
		tobeadded = 1024 - modded;
	}

	// Else if 1024-128 is less than modded
	else if (1024 - modded < 128) {
		tobeadded = 2048 - modded;
	}

	// Append 1 to string
	s1024 += "1";

	// Append tobeadded-129 zeros
	// in the string
	for (int y = 0; y < tobeadded - 129; y++) {
		s1024 += "0";
	}

	// Stores the binary representation
	// of string length
	string lengthbits
		= std::bitset<128>(orilen).to_string();

	// Append the lengthbits to string
	s1024 += lengthbits;

	// Find the count of chunks of
	// size 1024 each
	int blocksnumber = s1024.length() / 1024;

	// Stores the numbering of chunks
	int chunknum = 0;

	// Stores hash value of each blocks
	string Blocks[blocksnumber];

	// Traverse the string s1024
	for (int i = 0; i < s1024.length();
		i += 1024, ++chunknum) {
		Blocks[chunknum] = s1024.substr(i, 1024);
	}

	// Traverse the array Blocks[]
	for (int letsgo = 0;
		letsgo < blocksnumber;
		++letsgo) {

		// Divide the current string
		// into 80 blocks size 16 each
		separator(Blocks[letsgo]);

		AA = A;
		BB = B;
		CC = C;
		DD = D;
		EE = E;
		FF = F;
		GG = G;
		HH = H;

		int count = 0;

		// Find hash values
		for (int i = 0; i < 10; i++) {

			// Find the Hash Values

			Func(A, B, C, D, E, F, G, H, count);
			count++;
			Func(H, A, B, C, D, E, F, G, count);
			count++;
			Func(G, H, A, B, C, D, E, F, count);
			count++;
			Func(F, G, H, A, B, C, D, E, count);
			count++;
			Func(E, F, G, H, A, B, C, D, count);
			count++;
			Func(D, E, F, G, H, A, B, C, count);
			count++;
			Func(C, D, E, F, G, H, A, B, count);
			count++;
			Func(B, C, D, E, F, G, H, A, count);
			count++;
		}

		// Update the value of A, B, C,
		// D, E, F, G, H

		A += AA;
		B += BB;
		C += CC;
		D += DD;
		E += EE;
		F += FF;
		G += GG;
		H += HH;
	}

	stringstream output;

	// Print the hexadecimal value of
	// strings as the resultant SHA-512
	output << decimaltohex(A);
	output << decimaltohex(B);
	output << decimaltohex(C);
	output << decimaltohex(D);
	output << decimaltohex(E);
	output << decimaltohex(F);
	output << decimaltohex(G);
	output << decimaltohex(H);

	// Return the string
	return output.str();
}
void update(block* root, int bp){
	block * tmp= root;
	for (int i = 0; i < bp-1; i++){
		tmp= tmp->pnt;
	}
	tmp->Hash->left->left->hash=SHA512(tmp->trans_data);
	tmp->Hash->left->right->hash=SHA512(tmp->trans_data);
	tmp->Hash->left->hash=SHA512(tmp->Hash->left->right->hash+tmp->Hash->left->left->hash);
	tmp->Hash->hash=SHA512(tmp->Hash->left->hash+tmp->Hash->right->hash);
	while(tmp->pnt!=NULL){
		tmp->pnt->phash=tmp->Hash;
		tmp->pnt->Hash->right->right->hash=SHA512(tmp->pnt->phash->hash);
		tmp->pnt->Hash->right->hash=SHA512(tmp->pnt->Hash->right->left->hash+tmp->pnt->Hash->right->right->hash);
		tmp->pnt->Hash->hash=SHA512(tmp->pnt->Hash->left->hash+tmp->pnt->Hash->right->hash);
		tmp=tmp->pnt;
	}
}

mtnode* merkle_tree(block *root){ //function to create an initial merkle tree out of 4 transactions
    //struct merkle_tree_node** mt_arr =(struct merkle_tree_node*)malloc(8*sizeof(mtnode));
    //printf("test");
    struct merkle_tree_node* m1=new (mtnode);
    //printf("test");
    struct merkle_tree_node* m2=new (mtnode);
    //printf("test");
    struct merkle_tree_node* m3=new (mtnode);
 struct merkle_tree_node* m4=new (mtnode);
    struct merkle_tree_node* m5=new (mtnode);
    //printf("test");
    struct merkle_tree_node* m6=new (mtnode);
     struct merkle_tree_node* m7=new (mtnode);
    // printf("test_1");
    m1->hash=SHA512(root->trans_data);
    m2->hash=SHA512(root->trans_data);
    // printf("test_2");
    m3->hash=SHA512(to_string(root->block_no));
    // printf("test_3");
	if(root->phash==NULL){
		m4->hash=SHA512("NULL");
	}
	else{
    	m4->hash=SHA512(root->phash->hash);
	}
    // printf("test_3");
    m1->left=NULL;
    m1->right=NULL;
    m2->left=NULL;
    m2->right=NULL;
    m3->left=NULL;
    m3->right=NULL;
    m4->left=NULL;
    m4->right=NULL;
    m5->right=m2;
    m5->left=m1;
    m6->right=m4;
    m6->left=m3;
    m5->hash=SHA512(m1->hash+m2->hash);
    // printf("test");
    m6->hash=SHA512(m3->hash+m4->hash);
    // printf("test");
    m7->left=m5;
    m7->right=m6;
    m7->hash=SHA512(m6->hash+m5->hash);
    // printf("test");
    //Forming the complete merkle tree using hash=SHA512
    return m7;
}

struct block* transaction(int amt,string user1,string user2,struct block* root){
    // printf("t1\n");
    string tr;
    tr=user1+to_string(amt)+user2;
    if(root==NULL){
        root=new (struct block);
        root->phash=NULL;
        root->pnt=NULL;
        root->trans_data=tr;
        root->block_no = b_no/4+1;
        root->Hash=merkle_tree(root);
    }
    else {
        struct block* temp = root;
        while(temp->pnt!=NULL){
            temp=temp->pnt;
        }
        temp->pnt =new (struct block);
        temp->pnt->block_no=b_no/4+1;
        temp->pnt->phash=temp->Hash;
        temp->pnt->pnt=NULL;;
        temp->pnt->trans_data=tr;
		temp->pnt->Hash = merkle_tree(temp->pnt);
    }
	b_no++;
    return root;
}

int check(block * comparant, block * c1, block * c2, block * c3 ){
// here the comparant is the one which is to be compared to rest of the three local copies
// printf("t_4\n");
int counter = 0; // to check for majority
block * temp = comparant;
//temp = comparant;
while(temp->pnt!=NULL){  // just checking the last block
    temp = temp->pnt;
}
block * temp1 = c1;
//temp1 = c1;
while(temp1->pnt!=NULL){
    temp1 = temp1->pnt;
}
if(temp->Hash->hash == temp1->Hash->hash){
    counter++;
}
free(temp1);
block * temp2 = c2;
//temp2 = c2;
while(temp2->pnt!=NULL){
    temp2 = temp2->pnt;
}
if(temp->Hash->hash== temp2->Hash->hash){
    counter++;
}
free(temp2);

block * temp3 = c3;
//temp1 = c3;
while(temp3->pnt!=NULL){
    temp3 = temp3->pnt;
}
if(temp->Hash->hash== temp3->Hash->hash){
    counter++;
}
free(temp3);
return counter;
}

void print_BC(struct block* root){
	struct block* temp = root; 
    while(temp!=NULL){
        cout<<"X---------------------X"<<endl;
		cout<<"Block Number: "<< temp->block_no<<endl;
        //printf("Block Number: %d\n",temp->block_no);
        cout<<"X---------------------X"<<endl;
        cout<<"Transaction Data: "<<temp->trans_data<<endl;
		//cout<<"Block Number: "<< temp->block_no<<endl;
        //printf("%s\n",temp->trans_data);
        cout<<"X---------------------X"<<endl;
		cout<<"Current Hash: "<<temp->Hash->hash<<endl;
        //printf("Current Hash: %s\n",temp->Hash->hash);
        cout<<"X---------------------X"<<endl;
		if(temp->phash==NULL){cout<<"Previous Hash: NULL"<<endl;}
		else cout<<"Previous Hash: "<<temp->phash->hash<<endl;
        //printf("Previous Hash: %s\n",temp->phash->hash);
        cout<<"X---------------------X"<<endl;
        temp=temp->pnt;
    }
}

void compareNcopy(block* first, block* second){
    block* temp1 = first;
    block* temp2 = second;

    // Traverse both linked lists until you find the first differing node.
    while (temp1 != NULL && temp2 != NULL && temp1->Hash->hash == temp2->Hash->hash) {
        temp1 = temp1->pnt;
        temp2 = temp2->pnt;
    }

    // Start copying differing nodes from the second list.
    while (temp2 != NULL) {
        temp1->Hash->hash=temp2->Hash->hash;
        temp1->phash->hash=temp2->phash->hash;
        temp1->trans_data=temp2->trans_data;
        //newNode->pnt = NULL;

        /*if (result == NULL) {
            result = newNode;
            resultTail = newNode;
        } else {
            resultTail->pnt = newNode;
            resultTail = newNode;
        }*/

        temp2 = temp2->pnt;
        temp1 = temp1->pnt;
    }

    // Return the resulting linked list.
    //return result;
}

int main(){
    int ch;
    string u1,u2,u3,u4;
   // string amount,name1,name2;
    struct block* root1;
    struct block* root2;
    struct block* root3;
    struct block* root4;
    root1=NULL;
    root2=NULL;
    root3=NULL;
    root4=NULL;
    printf("Enter the username of all the 4 users:\nEnter Username_1: ");
    cin >> u1;
    printf("Enter Username_2: ");
    cin >> u2;
    printf("Enter Username_3: ");
    cin >> u3;
    printf("Enter Username_4: ");
    cin >> u4;
    while(1){
		printf("\n");
        printf("MENU:\n");
        printf("1.Enter details about a transaction\n2.Display the blockchain\n3.Try having fun by being a hacker :)\n4.Exit");
        printf("Enter your choice: ");
        cin >> ch;
        if(ch==1){
            string name1,name2;
            int amt;
            printf("Enter your username: ");
            cin >> name1;
            if(name1 == u1 || name1 == u2 || name1 == u3 || name1 == u4){
                printf("Enter the username with whom you want to do the transaction: ");
                cin >> name2;
                if((name2 == u1 || name2 == u2 || name2 == u3 || name2 == u4)){
                    //if((name2!=name1)){
                    printf("You are eligible to enter a transaction which would still be validated.\n");
                    printf("Enter transaction amount: ");
                   scanf("%d",&amt);
                    //}
                }
                else{ printf("Enter a valid username\n");
				continue;
				}
            }
            else{ printf("Enter a valid username\n");
			continue;}
            int valid_count = 0;
            char y_n;
            for(int i=1;i<=4;i++){
                printf("User %d, Enter your consensus for the transaction ( y or n ) ",i);
                scanf(" %c",&y_n);
                if(y_n == 'y'){
                    valid_count++;
                    // increasing the validation counter
                }
            }
            if(valid_count>2){
                printf("The transaction has been validated by the nodes in the network, hence has been added to the blockchain.\n");
				root1=transaction(amt,name1,name2,root1);
				root2=transaction(amt,name1,name2,root2);
				root3=transaction(amt,name1,name2,root3);
				root4=transaction(amt,name1,name2,root4);
            }
            else{
                printf("Your transaction has not been validated\n");
            }
			valid_count=0;
        }
        else if(ch==2){
            string name;
            printf("Enter the username whose local copy of blockchain you want to display: ");
            cin >> name;
            if(name == u1){
                print_BC(root1);
            }
            else if(name == u2){
                print_BC(root2);
            }
            else if(name == u3){
                print_BC(root3);
            }
            else if(name == u4){
                print_BC(root4);
            }
            else{
                printf("Enter a valid username.\n");
            }
        }   
        else if(ch==3){
            int bp;
			struct block* temp;
			string n1;
			printf("Enter the username whose local copy you want to manipulate:");
			cin>> n1;
			if(n1==u1)
			 temp=root1;
			else if(n1==u2)
			 temp=root2;
			else if(n1==u3)
			 temp=root3;
			else if(n1==u4)
			 temp=root4;
            printf("No of blocks in the chain are: %d\n",b_no/4);
			printf("Enter the block no. you want to manupilate:");
			scanf("%d",&bp);
			for(int i=0;i<bp-1;i++){
				temp=temp->pnt;
			}
			string nm1,nm2;
			int amt1;
			printf("Enter user 1 ");
            cin >> nm1;
			printf("Enter user 2 ");
			cin >> nm2;
			printf("Enter amount ");
			cin >> amt1;
            //string g1 = temp->trans_data;
			temp->trans_data=nm1+to_string(amt1)+nm2;
            /*while (temp!=NULL)
            {
                cout << temp->phash->hash << endl;
            }*/
			//update(root1,bp);
            if(n1==u1){
			    update(root1,bp);
				printf("Here is the local copy of blockchain after manupilation but before the checking");
				print_BC(root1);
				}
	       else if(n1==u2){
			    update(root2,bp);
				printf("Here is the local copy of blockchain after manupilation but before the checking");
				print_BC(root2);
				}
			else if(n1==u3){
			    update(root3,bp);
				printf("Here is the local copy of blockchain after manupilation but before the checking");
				print_BC(root3);
				}
			else if(n1==u4){
			    update(root4,bp);
				printf("Here is the local copy of blockchain after manupilation but before the checking");
				print_BC(root4);
				}
			 else cout<<"Enter a valid username";
            // now we updated the local copy as per hackers demand
            // we have to check this with rest of the local copies to get the majority
            // for the sake of checking we will check the last block of each local chain

            //printf("HI_THERE\n");
           int ctr1,ctr2,ctr3,ctr4;
			  ctr1 = check(root1,root2,root3,root4);
			 ctr2 = check(root2,root1,root3,root4);
			 ctr3 = check(root3,root2,root1,root4);
             ctr4 = check(root4,root2,root1,root3);
			 
            //// in our case only one local chain would be there which will not be having the majority
          if(ctr1==2){
                 printf("The data in local chain 1 matches with the majority\n");
             }
             else{
                 printf("There exists no majority so the changes you made is not valid\n");
                 printf("Thus the changes you made in local chain 1 would be reversed\n");
                 compareNcopy(root1,root2);
             }

             if(ctr2==2){
                 printf("The data in local chain 2 matches with the majority\n");
             }
             else{
                 printf("There exists no majority so the changes you made is not valid\n");
                 printf("Thus the changes you made in local chain 2 would be reversed\n");
                 compareNcopy(root2,root3);
             }

             if(ctr3==2){
                 printf("The data in local chain 3 matches with the majority\n");
             }
             else{
                 printf("There exists no majority so the changes you made is not valid\n");
                 printf("Thus the changes you made in local chain 3 would be reversed\n");
                 compareNcopy(root3,root4);
             }

             if(ctr4==2){
                 printf("The data in local chain 4 matches with the majority\n");
             }
             else{
                 printf("There exists no majority so the changes you made is not valid\n");
                 printf("Thus the changes you made in local chain 4  would be reversed\n");
                 compareNcopy(root4,root1);
             }
            free(temp);
        }
	else if(ch==4)
	break;
	else {
		printf("Enter a valid input");
		continue;
	}
    }
    return 0;
}