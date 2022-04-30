/* 
Adithya Subramani R,
2020506006,IT dept,MIT campus


*/

//header files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<windows.h>
#include<time.h>
#include<conio.h>


//all structures

//hash table
struct table{
   long data;   
   int key;
   struct table *next;
};

//BST
struct tree{
    long acc_no,ph_no;
    char name[30],acc_type[10],dob[11],date[11],address[20],gender[10];
    float balance;
    struct tree *left, *right;
};

//linked list
struct list{
    long sen_acc_no,rec_acc_no;
    float amount;
    char tr_date[30];
    struct list* next;
};

struct list* linkedList = NULL;
//all functions

struct tree* search(struct tree* root, long ser_acc_no){
	if (root == NULL || root->acc_no == ser_acc_no) return root;
	if (root->acc_no < ser_acc_no) return search(root->right, ser_acc_no);
	return search(root->left, ser_acc_no);
}

void disp_account(struct tree* node,long new_acc_no){
    struct tree* temp = search(node,new_acc_no);
    if(temp==NULL) printf("\nCustomer not found...");
    else{
        printf("\nAccount number: %ld",temp->acc_no);
        printf("\nAccount holder's name: %s",temp->name);
        printf("\nAccount holder's address: %s",temp->address);
        printf("\nAccount holder's phone number: %ld",temp->ph_no);
        printf("\nAccount holder's date of birth: %s",temp->dob);
        printf("\nDate of creation of account: %s",temp->date);
        printf("\nAccount type: %s",temp->acc_type);
        printf("\nGender: %s",temp->gender);
        printf("\nAccount balance: %.2f",temp->balance);
    }

}

struct tree* upd_account(struct tree* node,long new_acc_no,long new_ph_no,char new_name[50],char new_address[60]){
    struct tree* temp = search(node,new_acc_no);
    if(temp==NULL){
      printf("\nCustomer not found...");
      return node;
    }
    temp->ph_no = new_ph_no;
    strcpy(temp->name,new_name);
    strcpy(temp->address,new_address);
    printf("\nCustomer details updated succesfully...");
    return node;
}

struct tree* add_account(struct tree* node,long new_acc_no,long new_ph_no,char new_name[50],char new_acc_type[10],char new_dob[11],char new_date[11],char new_address[60],char new_gender[10],float new_balance){
    if(search(node,new_acc_no)!=NULL){
      printf("\nAn account with the given account number already exists....");
      return node;
    }
    if (node == NULL){
        struct tree* temp= (struct tree*)malloc(sizeof(struct tree));
        temp->acc_no = new_acc_no;
        temp->ph_no = new_ph_no;
        temp->balance = new_balance;
        strcpy(temp->name,new_name);
        strcpy(temp->acc_type,new_acc_type);
        strcpy(temp->dob,new_dob);
        strcpy(temp->date,new_date);
        strcpy(temp->address,new_address);
        strcpy(temp->gender,new_gender);
        temp->left = temp->right = NULL;
        printf("\nCustomer details added succesfully...");
        return temp;
    }
    else if (new_acc_no < node->acc_no){
        node->left = add_account(node->left,new_acc_no,new_ph_no,new_name,new_acc_type,new_dob,new_date,new_address,new_gender,new_balance);
    }
    else if (new_acc_no > node->acc_no){
        node->right = add_account(node->right,new_acc_no,new_ph_no,new_name,new_acc_type,new_dob,new_date,new_address,new_gender,new_balance);
    }
    printf("\nCustomer details added succesfully...");
    return node;
}

struct tree *minValueNode(struct tree *node) {
  struct tree *current = node;
  while (current && current->left != NULL)
    current = current->left;
  return current;
}


struct tree *del_account(struct tree *root, long del_acc_no) {
  if (root == NULL){
    printf("\nCustomer not found...");
    return root;
  } 
  if (del_acc_no < root->acc_no)
    root->left = del_account(root->left, del_acc_no);
  else if (del_acc_no > root->acc_no)
    root->right = del_account(root->right, del_acc_no);

  else {
    if (root->left == NULL) {
      struct tree *temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      struct tree *temp = root->left;
      free(root);
      return temp;
    }
    struct tree *temp = minValueNode(root->right);
    root->acc_no = temp->acc_no;
    root->right = del_account(root->right, temp->acc_no);
  }
  printf("\nCustomer details deleted succesfully...");
  return root;
}

void disp_all(struct tree* root){
    if (root != NULL) {
        disp_all(root->left);
        printf("\033[0;33m");
        printf("\n| %-5ld| %-20s| %-15s| %-10ld| %-10s| %-10s| %-10s| %-10s| %-10.2f| ",root->acc_no,root->name,root->address,root->ph_no,root->dob,root->acc_type,root->gender,root->date,root->balance);
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("\033[0m");
        disp_all(root->right);
    }
}



struct tree* upd_bal(struct tree* root,long acc_no,float diff,int stt){
    struct tree* temp = search(root,acc_no);
    if(stt==1) temp->balance = temp->balance+diff;
    else if(stt==2) temp->balance = temp->balance-diff;
    return root;
}

struct tree* transact(struct tree* node, long sen_acc,long rec_acc,float amt,char date[30]){
    struct list* new_list = (struct list*) malloc(sizeof(struct list));
    struct list *last = linkedList;  
    struct tree* sen_tree = search(node,sen_acc);
    struct tree* rec_tree = search(node,rec_acc);
    if(!sen_tree||!rec_tree) {
      printf("\nSender or Reciever not found!");
      return node;
    }
    else if(sen_tree->balance<amt){
      printf("\nInsufficient balance");
      return node;
    }
    new_list->sen_acc_no  = sen_acc;
    new_list->rec_acc_no  = rec_acc;
    new_list->amount = amt;
    strcpy(new_list->tr_date,date);
    new_list->next = NULL;
    node = upd_bal(node,sen_acc,amt,2);
    node = upd_bal(node,rec_acc,amt,1);
    if (linkedList == NULL){
       linkedList = new_list;
       return node;
    } 
    while (last->next != NULL)
        last = last->next;
    last->next = new_list;
    return node;   
}

struct tree* withdraw(struct tree* node,long acc,float amt,char date[30]){
    struct list* new_list = (struct list*) malloc(sizeof(struct list));
    struct list *last = linkedList;  
    struct tree* sen_tree = search(node,acc);
    if(!sen_tree) {
      printf("\nAccount not found!");
      return node;
    }
    else if(sen_tree->balance<amt){
      printf("\nInsufficient balance");
      return node;
    }
    new_list->sen_acc_no  = acc;
    new_list->rec_acc_no  = -1;
    new_list->amount = amt;
    strcpy(new_list->tr_date,date);
    new_list->next = NULL;
    node = upd_bal(node,acc,amt,2);
    if (linkedList == NULL){
       linkedList = new_list;
       return node;
    } 
    while (last->next != NULL)
        last = last->next;
    last->next = new_list;
    return node;   
}

struct tree* deposit(struct tree* node,long acc,float amt,char date[30]){
    struct list* new_list = (struct list*) malloc(sizeof(struct list));
    struct list *last = linkedList;  
    struct tree* rec_tree = search(node,acc);
    if(!rec_tree) {
      printf("\nAccount not found!");
      return node;
    }
    new_list->sen_acc_no  = -1;
    new_list->rec_acc_no  = acc;
    new_list->amount = amt;
    strcpy(new_list->tr_date,date);
    new_list->next = NULL;
    node = upd_bal(node,acc,amt,1);
    if (linkedList == NULL){
       linkedList = new_list;
       return node;
    } 
    while (last->next != NULL)
        last = last->next;
    last->next = new_list;
    return node;   
}

void insertaccIntoFile(struct tree* root,FILE *add_acc){
    if (root != NULL) {
        insertaccIntoFile(root->left,add_acc);
        fprintf(add_acc,"%ld %s %s %ld %s %s %s %s %.2f\n",root->acc_no,root->name,root->address,root->ph_no,root->dob,root->acc_type,root->gender,root->date,root->balance);
        insertaccIntoFile(root->right,add_acc);
    }
}

void dispTrans(){
  struct list* temp = linkedList;
  while(temp!=NULL){
    printf("%ld %ld %.2f %s\n",temp->sen_acc_no,temp->rec_acc_no,temp->amount,temp->tr_date);
    temp =  temp->next;
  }
}

void dispAllTrans(){
  FILE *disp_trans;
  disp_trans = fopen("transaction.txt","r");
  struct list* temp = malloc(sizeof(struct list));
  while(fscanf(disp_trans,"%ld %ld %f %s\n",&temp->sen_acc_no,&temp->rec_acc_no,&temp->amount,&temp->tr_date)!=EOF){
    printf("\033[0;33m");
    printf("\n| %-15ld| %-15ld| %-10.2f| %-20s|",temp->sen_acc_no,temp->rec_acc_no,temp->amount,temp->tr_date);
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\033[0m");
  }
  fclose(disp_trans);
}

void saveToFile(struct tree* root){
    FILE *add_acc,*add_trans;
    add_acc = fopen("accounts.txt","w");
    add_trans = fopen("transaction.txt","a+");
    insertaccIntoFile(root,add_acc);
    struct list* temp = linkedList;
    while(temp!=NULL){
      fprintf(add_trans,"%ld %ld %.2f %s\n",temp->sen_acc_no,temp->rec_acc_no,temp->amount,temp->tr_date);
      temp=temp->next;
    }
    printf("\nData added to the file successfully...");
    fclose(add_acc);
    fclose(add_trans);
    return;
}


//main function
int main(){
  char new_date[11],trrr_date[30];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(new_date, sizeof(new_date)-1,"%d/%m/%Y", t);
  
  strftime(trrr_date, sizeof(trrr_date)-1,"%d/%m/%Y-%H:%M", t);
  int ch;
  long new_acc_no,new_acc_no1,new_ph_no;
  char new_name[30],new_acc_type[10],get_date[11],new_dob[11],new_address[20],new_gender[10];
  float new_balance;
  struct tree *root=NULL;
  FILE *add_acc;
  add_acc = fopen("accounts.txt","r");
  while(fscanf(add_acc,"%ld %s %s %ld %s %s %s %s %f\n",&new_acc_no,&new_name,&new_address,&new_ph_no,&new_dob,&new_acc_type,&new_gender,&get_date,&new_balance)!=EOF){
    root=add_account(root,new_acc_no,new_ph_no,new_name,new_acc_type,new_dob,get_date,new_address,new_gender,new_balance);
  }
  fclose(add_acc);
  do{
    system("cls");
    printf("\033[1;36m");
    printf("%-46s","\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    printf("%-46s","\n\xB2 \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 \xB2");
    printf("%-46s","\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    printf("\n\xB2 %-46s \xB2","1. CREATE A NEW ACCOUNT.");
    printf("\n\xB2 %-46s \xB2","2. DISPLAY ACCOUNT DETAILS BY ACCOUNT NUMBER.");
    printf("\n\xB2 %-46s \xB2","3. UPDATE ACCOUNT DETAILS BY ACCOUNT NUMBER.");
    printf("\n\xB2 %-46s \xB2","4. DELETE ACCOUNT DETAILS BY ACCOUNT NUMBER.");
    printf("\n\xB2 %-46s \xB2","5. INSERT TRANSACTION DETAILS.");
    printf("\n\xB2 %-46s \xB2","6. WITHDRAWAL DETAILS.");
    printf("\n\xB2 %-46s \xB2","7. DEPOSIT DETAILS.");
    printf("\n\xB2 %-46s \xB2","8. DISPLAY ALL TRANSACTIONS.");
    printf("\n\xB2 %-46s \xB2","9. SAVE ALL CHANGES TO THE FILE.");
    printf("\n\xB2 %-46s \xB2","10. DISPLAY ALL ACCOUNT DETAILS.");
    printf("\n\xB2 %-46s \xB2","11. EXIT.");
    printf("%-46s","\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    printf("\033[0m");
    printf("\n\nEnter your choice:");
    scanf("%d",&ch);
    system("cls");
    switch(ch){
      case 1:
      printf("Account number: ");
      scanf("%ld",&new_acc_no);
      printf("Account holder's name: ");
      scanf("%s",new_name);
      printf("Account holder's address: ");
      scanf("%s",new_address);
      printf("Account holder's phone number: ");
      scanf("%ld",&new_ph_no);
      printf("Account holder's date of birth: ");
      scanf("%s",new_dob);
      printf("Account type: ");
      scanf("%s",new_acc_type);
      printf("Gender: ");
      scanf("%s",new_gender);
      printf("Account balance (Initial deposit): ");
      scanf("%f",&new_balance);
      root=add_account(root,new_acc_no,new_ph_no,new_name,new_acc_type,new_dob,new_date,new_address,new_gender,new_balance);
      printf("\n");
      system("pause");
      break;
      case 2:
      printf("Account number: ");
      scanf("%ld",&new_acc_no);
      disp_account(root,new_acc_no);
      printf("\n");
      system("pause");
      break;
      case 3:
      printf("Account number: ");
      scanf("%ld",&new_acc_no);
      printf("Account holder's name: ");
      scanf("%s",new_name);
      printf("Account holder's address: ");
      scanf("%s",new_address);
      printf("Account holder's phone number: ");
      scanf("%ld",&new_ph_no);
      root=upd_account(root,new_acc_no,new_ph_no,new_name,new_address);
      printf("\n");
      system("pause");
      break;    
      case 4:
      printf("Account number: ");
      scanf("%ld",&new_acc_no);      
      root=del_account(root,new_acc_no);
      printf("\n");
      system("pause");
      break;
      case 5:
      printf("Sender's account number :");
      scanf("%ld",&new_acc_no);
      printf("Receiver's account number :");
      scanf("%ld",&new_acc_no1);
      printf("Amount :");
      scanf("%f",&new_balance);
      root=transact(root,new_acc_no,new_acc_no1,new_balance,trrr_date);
      printf("\n");
      system("pause");
      break;
      case 6:
      printf("Account number :");
      scanf("%ld",&new_acc_no);
      printf("Amount :");
      scanf("%f",&new_balance);
      root=withdraw(root,new_acc_no,new_balance,trrr_date);
      printf("\n");
      system("pause");
      break;
      case 7:
      printf("Account number :");
      scanf("%ld",&new_acc_no);
      printf("Amount :");
      scanf("%f",&new_balance);
      root=deposit(root,new_acc_no,new_balance,trrr_date);
      printf("\n");
      system("pause");
      break;
      case 8:
      printf("\033[0;32m");
      printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      printf("\n| %-15s| %-15s| %-10s| %-20s|","SEN. ACC. NO","REC. ACC. NO","AMONUT","TRANS. DATE");
      printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      printf("\033[0m");
      dispAllTrans();
      printf("\n");
      system("pause");
      break; 
      case 9:
      saveToFile(root);
      printf("\n");
      system("pause");
      break;
      case 10:
      printf("\033[0;32m");
      printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      printf("\n| %-5s| %-20s| %-15s| %-10s| %-10s| %-10s| %-10s| %-10s| %-10s| ","AC_NO","NAME","ADDRESS","PHONE NUME","DOB","ACC TYPE","GENDER","CR. DATE","BALANCE");
      printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      printf("\033[0m");
      disp_all(root);
      printf("\n");
      system("pause");
      break;
    }

  }while(ch!=11);
}

