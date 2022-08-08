//dsa project 
// This is a COMMAND LINE INTERFACE BASED application which simulates a Online Shopping Application.

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

typedef struct usernames
{
    char user[30];
}usernames;

typedef struct passwords
{
    char pass[30];
}passwords;

typedef struct product_details
{
    char brand[30];
    char name[30];
    int price;
    char category[30];
}pd;

typedef struct priority_queue
{
    int node;
    int wt;//consideration factor which will consider all traffic weather and edge length
    int weight_covered;//this will only consider the edge length without any other factors
}priority_queue;

typedef struct nodes
{
    int traffic;
    int edge_weight;
    int weather;
}nodes;

int str_to_num(char str[])
{
    //this function returns the integer equivalent of the given string
    int num=0;
    for(int i=0;str[i]!='\0';i++)
    {
        num=(num*10)+(str[i]-'0');
    }
    return num;
}

int isEmpty(priority_queue *pq,int n)
{
    //returns if priority queue is empty or not
    for(int i=0;i<n;i++)
    {
        if(pq[i].wt!=-1 || pq[i].node!=-1)
        return 0;
    }
    return 1;
}

priority_queue front(priority_queue *pq,int n)
{
    //returns the node with least edge weight
    int w=INT_MAX;
    priority_queue temp;
    for(int i=0;i<n;i++)
    {
        if(pq[i].wt<w && pq[i].wt!=-1)
        {
            w=pq[i].wt;
            temp=pq[i];
        }
    }
    return temp;
}

void dequeue(priority_queue *pq,int n,priority_queue temp)
{
    //deletes the minimum most element from the priority queue
    //in other words we will mark that particular node and weight with useless value like -1
    for(int i=0;i<n;i++)
    {
        if(temp.node==pq[i].node && temp.wt==pq[i].wt)
        {
            pq[i].wt=-1;
            pq[i].node=-1;
        }
    }
}

void dijkstra(int dest)
{
    //function for dijkstra's algorithm
    //this function finds shortest path to deliver product after considering all 3 factors i.e traffic,weather and edge weight
    priority_queue pq[200];
    for(int i=0;i<200;i++)
    {
        pq[i].node=-1;
        pq[i].wt=-1;
    }

    int l=0;
    int src=0;//assuming the shop is at 0th node
    char num[20];
    FILE *ptr=fopen("graph_file.txt","r");
    fscanf(ptr,"%s",num);
    int n=str_to_num(num);
    //reading the map from the file
    nodes **arr=(nodes **)malloc(sizeof(nodes *)*n);
    for(int i=0;i<n;i++)
    {
        arr[i]=(nodes *)malloc(sizeof(nodes)*n);
        for(int j=0;j<n;j++)
        {
            char edge_weight[20];
            char traffic[20];
            char weather[20];
            fscanf(ptr,"%s",edge_weight);
            fscanf(ptr,"%s",traffic);
            fscanf(ptr,"%s",weather);
            arr[i][j].edge_weight=str_to_num(edge_weight);
            arr[i][j].traffic=str_to_num(traffic);
            arr[i][j].weather=str_to_num(weather);
            //printf("( %d %d %d ) ",arr[i][j].edge_weight,arr[i][j].traffic,arr[i][j].weather);
        }
        //printf("\n");
    }
    //the distance array will also be modifies as it takes all factors into consideration
    //the first row will store all the weights including consideration factor
    //the second row will store only the distance
    int distance[2][n];
    for(int i=0;i<n;i++)
    {
        distance[0][i]=772681;
        distance[1][i]=0;
    }
    distance[0][src]=0;
    pq[l].node=src;
    pq[l].wt=0;
    pq[l].weight_covered=0;

    while(!isEmpty(pq,l+1))
    {
        priority_queue temp=front(pq,l+1);
        dequeue(pq,l+1,temp);

        int node=temp.node;
        int curr_dist=temp.wt;
        int weight_covered_till_now=temp.weight_covered;
        for(int i=0;i<n;i++)
        {
            //considering all factors
            int next_dist=curr_dist+(arr[node][i].traffic+arr[node][i].weather+arr[node][i].edge_weight);
            if(arr[node][i].edge_weight!=0 && next_dist<distance[0][i])
            {
                distance[0][i]=next_dist;
                l=l+1;
                pq[l].node=i;
                pq[l].wt=next_dist;
                pq[l].weight_covered=weight_covered_till_now+arr[node][i].edge_weight;
                distance[1][i]=pq[l].weight_covered;
            }
        }
    }
    printf("\n\nShortest distance between shop and your location is %d KM\n\n",distance[0][dest]);
    //assuming that to travel consideration factor 1 it takes 1.5 minute of time
    printf("\nTotal time for product to be delivered is %f minutes\n",distance[0][dest]*1.5);
}


void select_product()
{

    //https://stackoverflow.com/questions/21071127/c-file-handling-record-search-last-record-displayed-twice
    //the error with file explained (that is why the last added item is printed twice)
    FILE *ptr=fopen("items.txt","r");
    char number_of_items[10];
    fscanf(ptr,"%s",number_of_items);
    int num=str_to_num(number_of_items);
    pd products[num];
    int l=0;
    while(!feof(ptr))
    {
        fscanf(ptr,"%s",products[l].brand);
        fscanf(ptr,"%s",products[l].name);
        char price[10];
        fscanf(ptr,"%s",price);
        products[l].price=str_to_num(price);
        fscanf(ptr,"%s",products[l].category);
        l=l+1;
    }
    char search_query[40];
    fflush(stdin);
    system("clear");
    printf("\nSearch in the store : \n");
    scanf("%s",search_query);
    fflush(stdin);
    printf("\n");
    int flag_succesful_search=0;//flag to denote succesfull search
    for(int i=0;i<num;i++)
    {
        if(strcasecmp(search_query,products[i].brand)==0||strcasecmp(search_query,products[i].name)==0||strcasecmp(search_query,products[i].category)==0)
        {
            printf("%d %s %s Rs. %d\n",i,products[i].brand,products[i].name,products[i].price);
            flag_succesful_search=1;
        }
    }
    if(flag_succesful_search==1){
    printf("\n\nPress the product_code in front of result to buy.\n\nor other keys to go back\t :\n");
    int product_code;
    scanf("%d",&product_code);
    if(product_code>=num || product_code<0)
    {
        select_product();
    }
    else
    {
    FILE *bill=fopen("final_bill.txt","a");
    int qty;
    printf("\nEnter the quantity of the product : \n");
    scanf("%d",&qty);
    fprintf(bill,"%d %s %s %d %lld .\n",product_code,products[product_code].brand,products[product_code].name,qty,products[product_code].price*qty);
    fclose(bill);
    system("clear");
    int c_or_ex;
    printf("Press 1 to continue shopping...\n");
    printf("Press 2 or other keys to exit.\n");
    printf("Please enter your choice : ");
    scanf("%d",&c_or_ex);
    printf("\n\n");
    if(c_or_ex==1)
    {
        select_product();
    }
    else
    {
        system("clear");
        printf("\nYour final bill is : \n");
        long long int total=0;
        bill=fopen("final_bill.txt","r");
        printf("\n\n\nItem_Code\tBrand\t\tProduct\t\t\tQuantity\t\tPrice\n");
        pd product_recommendations[4];
        int flag_for_recc=0;//flag to denote recommendation
        int index_rec=0;
        while(!feof(bill))
        {
            char prod_code[10];
            char prod_brand[30];
            char prod_name[30];
            char prod_qty[30];
            char prod_price[30];
            char garbage_read[10];
            fscanf(bill,"%s",prod_code);
            fscanf(bill,"%s",prod_brand);
            fscanf(bill,"%s",prod_name);
            fscanf(bill,"%s",prod_qty);
            fscanf(bill,"%s",prod_price);
            fscanf(bill,"%s",garbage_read);
            printf("%s\t%s\t\t%s\t\t\t%s\t\t%s\n",prod_code,prod_brand,prod_name,prod_qty,prod_price);
            if(flag_for_recc==0)
            {
                for(int i=0;i<num;i++)
                {
                    if(str_to_num(prod_code)!=i)
                    {
                        //recommending some other item not the same
                        if(index_rec<2)
                        {
                            //we will recommend 2 items of same brand
                            if(strcasecmp(prod_brand,products[i].brand)==0)
                            {
                                product_recommendations[index_rec]=products[i];
                                index_rec+=1;
                            }
                        }
                        else if(index_rec<4)
                        {
                            //2 items of same category but of different brand
                            char p_category[30];
                            strcpy(p_category,products[str_to_num(prod_code)].category);
                            if(strcasecmp(p_category,products[i].category)==0 && strcmp(prod_brand,products[i].brand)!=0)
                            {
                                product_recommendations[index_rec]=products[i];
                                index_rec+=1;
                            }
                        }
                    }
                }
                flag_for_recc=1;
            }
            total=total+str_to_num(prod_price);
        }
        fclose(bill);
        for(int i=0;i<50;i++)
        printf("-*-");
        printf("\nYour total bill is : Rs. %lld",total);
        printf("\n\n\nYou may consider buying the following products.\n\n");
        printf("\n\n\nBrand\t\tProduct\t\tCategory\t\tPrice\n");
        for(int i=0;i<index_rec;i++)
        printf("%s\t\t%s\t\t%s\t\t\tRs. %d\n",product_recommendations[i].brand,product_recommendations[i].name,product_recommendations[i].category,product_recommendations[i].price);
        printf("\n\nPlease enter your location code : ");
        int loc;
        scanf("%d",&loc);
        printf("\n\n\nYour products are out for delivery.......\n\n");
        dijkstra(loc);
    }
    }
    }
    else
    {
        printf("\nSearch unsuccesful.....\nPlease try again....");
        select_product();
    }
}

void user_interface()
{
    int choice;
    printf("\nPress 1 to login.\n");
    printf("\nPress 2 to signup.\n");
    scanf("%d",&choice);
    if(choice==1)
    {
        //first we are counting total number of users in the file 
        FILE *ptr=fopen("usernames.txt","r");
        int c=0;
        while(!feof(ptr))
        {
            char str[30];
            fscanf(ptr,"%s",str);
            c++;
        }
        fclose(ptr);
        ptr=NULL;
        //then we are creating an array to store all the usernames and then we are creating an array to store passwords corresponding to that
        //usernames
        usernames u[c];
        passwords p[c];
        ptr=fopen("usernames.txt","r");
        int l=0;
        while(!feof(ptr))
        {
            char str[30];
            fscanf(ptr,"%s",u[l].user);
            l=l+1;
        }
        ptr=fopen("passwords.txt","r");
        l=0;
        while(!feof(ptr))
        {
            char str[30];
            fscanf(ptr,"%s",p[l].pass);
            l=l+1;
        }
        char user_name[30];
        fflush(stdin);
        system("clear");
        printf("Please enter the usernme : ");
        scanf("%s",user_name);
        //we will search in the array of usernames whether username exists or not
        int flag_to_denote_user=0;
        int index=-1;
        for(int i=0;i<c;i++)
        {
            if(strcasecmp(user_name,u[i].user)==0)
            {
                flag_to_denote_user=1;
                index=i;
                break;
            }
        }
        if(flag_to_denote_user==1)
        {
            char pass[30];
            printf("\nPlease enter password : \n");
            scanf("%s",pass);
            if(strcmp(pass,p[index].pass)==0)
            {
                printf("\nLogin Succesful.");
                FILE *bill=fopen("final_bill.txt","w");
                fclose(bill);
                select_product();
            }
            else
            {
                printf("\nWrong Password.");
            }
        }
        else
        {
            printf("\nNo such username exists.");
        }
    }
    else if(choice==2)
    {
        //for signup first we will check if an user with same username exists or not

        //first we are counting total number of users in the file 
        FILE *ptr=fopen("usernames.txt","r");
        int c=0;
        while(!feof(ptr))
        {
            char str[30];
            fscanf(ptr,"%s",str);
            c++;
        }
        fclose(ptr);
        ptr=NULL;
        //then we are creating an array to store all the usernames and then we are creating an array to store passwords corresponding to that
        //usernames
        usernames u[c];
        ptr=fopen("usernames.txt","r");
        int l=0;
        while(!feof(ptr))
        {
            char str[30];
            fscanf(ptr,"%s",u[l].user);
            l=l+1;
        }
        fclose(ptr);
        printf("\nPlease enter a user name : \n");
        char user_name[30];
        scanf("%s",user_name);
        int flag=0;//flag to denote already existing user
        for(int i=0;i<c;i++)
        {
            if(strcasecmp(user_name,u[i].user)==0)
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
        {
            printf("\n\nSORRY !! the user name already exists.\n");
            printf("\nTry again with different user name.\n");
        }
        else
        {
            char pass[30];
            char confirm_pass[30];
            printf("\n\nPlease enter your password : ");
            scanf("%s",pass);
            printf("\n\nPlease confirm your password : ");
            scanf("%s",confirm_pass);
            if(strcmp(pass,confirm_pass)==0)
            {
                printf("\n\nUser registered succesfully.\n\n");
                FILE *fp=fopen("usernames.txt","a");
                FILE *fpp=fopen("passwords.txt","a");
                fprintf(fp,"\n%s\n",user_name);
                fprintf(fpp,"\n%s\n",pass);
                FILE *bill=fopen("final_bill.txt","w");
                fclose(bill);
                select_product();
            }
            else
            {
                printf("\n\nPassword did not match.\n\n");
            }
        }

    }
}
int main()
{
    user_interface();
}
