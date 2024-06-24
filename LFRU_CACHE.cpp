
// BY Vraj Dobariya 
// Date: 30th May 2024
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef long double lld;
typedef unsigned long long ull;

#define int ll
// just to scan input from input.txt and print output in output.tx
// void init_code()
// {
//     #ifndef ONLINE_JUDGE
//     freopen("input.txt", "r", stdin);
//     freopen("output.txt", "w", stdout);
//     #endif // ONLINE_JUDGE
// }



class Node{
     
     public:
     int key;
     int val;
     Node* next;
     Node* prev;

     // default constructor
     Node()
     {
        this->next=NULL;
        this->prev=NULL;
     }
    // new node constructor
     Node(int key,int val)
     {
        this->key = key;
        this->val = val;
        this->next=NULL;
        this->prev=NULL;
     }
};
// just for printing purpose
string s(21,'-');
const int DEFAULT_LRU_SIZE = 1000;
class LRU_CACHE{
public:
    Node* head;
    Node* tail;
    int LRU_MAX_CACHE_SIZE;
    int Curr_Size;
    unordered_map<int,Node*>Hashmap;
    // default constructor
    LRU_CACHE()
    {
        head=new Node(-1,-1);
        tail=new Node(-1,-1);
        head->next=tail;
        tail->prev=head;
        LRU_MAX_CACHE_SIZE=DEFAULT_LRU_SIZE;
        Curr_Size=0;
 //    head ------> tail
 //    head <------ tail
    }
    LRU_CACHE(int LRU_MAX_CACHE_SIZE)
    {
        head=new Node(-1,-1);
        tail=new Node(-1,-1);
        head->next=tail;
        tail->prev=head;
        this->LRU_MAX_CACHE_SIZE=LRU_MAX_CACHE_SIZE;
        this->Curr_Size=0;
        Hashmap.clear();
// initialization by constructor
 //    head ------> tail
 //    head <------ tail
    }
    

    void setCacheSize(int size_)
    {
            LRU_MAX_CACHE_SIZE=size_;
    }
    void PrintCurrentCache()
    {
        
        cout<<"\n";
        for(int i=0;i<LRU_MAX_CACHE_SIZE+1;i++)
        cout<<s;
        cout<<"\n";
        
        Node* curr = head;
        
        while(curr!=tail)
        {
            if(curr==head)
            {
                cout<<"   => { head <--->";
            }
            else
            cout<<" [k:"<<curr->key<<", v:"<<curr->val<<"] <--->";
            
            curr=curr->next;
        }
           
        
        cout<<" tail } <=";
        cout<<"\n";
        for(int i=0;i<LRU_MAX_CACHE_SIZE+1;i++)
        cout<<s;
        cout<<"\n";
    }

    
    Node* insertafterHead(int key,int value)
    {

        /*

        before insertion after head
        head ---> nextnode .... ---> tail
        head <--- nextnode .... <--- tail

        after insertion after head
        head ---> newNode ---> nextnode .... ---> tail
        head <--- newNode <--- nextnode .... <--- tail

        */
        
        Curr_Size++;

        Node* nextnodeafterhead=head->next;
       
        Node* newNode = new Node(key,value);

        // inserting or updating in Hashmap with key
        Hashmap[key]=newNode;

        head->next = newNode;
        newNode->prev=head;
        newNode->next=nextnodeafterhead;
        nextnodeafterhead->prev=newNode;
        
      return newNode;
    }


    void deletebeforeTail()
    {
         
        /*

        tail before deletion 
        head ---> .... prevnode ---> deleteNode ---> tail
        head <--- .... prevnode <--- deleteNode <--- tail

        tail after deletion
        head ---> .... prevnode ---> tail
        head <--- .... prevnode <--- tail

        */


       Curr_Size--;
       Node* deleteNode = tail->prev;
       Node* prevNode = deleteNode->prev;

       int key = deleteNode->key;

       // removing key from Hashmap
       Hashmap.erase(Hashmap.find(key));

       
       // assigning pointers correctly
       prevNode->next = tail;
       tail->prev= prevNode;

       delete deleteNode;


      
    }

    void deleteNode(int key)
    {
        /*
        before deletion 

        .... prevNode ---> deleteNode ---> nextNode ....
        .... prevNode <--- deleteNode <--- nextNode ....


        after  deletion

        .... prevNode ---> nextNode ....
        .... prevNode <--- nextNode ....
        
        */

        // this function is called only when we are sure that our key node is present so will not lead to any ambiguity.

        // removing key from Hashmap
        
        Curr_Size--;
        // getting prevnode of the node to be deleted
       

        Node* deleteNode = Hashmap[key];

        Node* prevNode = deleteNode->prev;
        
        // assigning pointers
        Node* afterdeleteNode = deleteNode->next;
        prevNode->next = afterdeleteNode;
        afterdeleteNode->prev = prevNode;
        Hashmap.erase(Hashmap.find(key));

        delete deleteNode;

       
    }

    int get(int key)
    {
        /*

        LRU before get

        head --> ..... keynode ... --> tail
        head <-- ..... keynode ... <-- tail

        LRU after get 
        1)gets deleted from original position
        2)gets inserted after head making the most recently used

        head --> keynode ... --> tail
        head <-- keynode ... <-- tail



        */
        
        if(Hashmap.find(key)==Hashmap.end())
        {
            // if key not found in Hashmap return -1 as not present in LRU
            return -1;
        }
        else
        {
          
            // if key found in Hashmap then we need to update the LRU.
            // => delete key value and insert the same after head

            int returnval = Hashmap[key]->val;
            // delete and insert to get updated LRU
            deleteNode(key);
            
            insertafterHead(key,returnval);

           return returnval;

        }
        
        
    }


    void put(int key,int value)
    {
            if(Hashmap.find(key)!=Hashmap.end())
            {
                // if already present in Hashmap then delete node and insert at head as last recent used
                deleteNode(key);
                insertafterHead(key,value);
            }
            else{
               
                if(Curr_Size<LRU_MAX_CACHE_SIZE)
                {
                    // if current size if less than max size then insert after head directly
                    insertafterHead(key,value);
                }
                else
                {
                    // size if maximized so delete least recently used and insert new node after head
                    deletebeforeTail();
                    insertafterHead(key,value);
                }
            }
    }
    
    
    Node* giveNodebeforeTail()
    {
        // gives previous Node of the tail.
        return tail->prev;
    }


};

const int DEFAULT_SIZE = 10010;
class LFUCache {
public:
    // basically LFU uses LRU as a Building Block.
    LRU_CACHE LFU[DEFAULT_SIZE];
    int Capacity;
    int Curr_Size;
    unordered_map<int,Node*>Hashmap;
    unordered_map<int,int>FreqOfKey;
    map<int,int>CountOfFreq;

    LFUCache(int Capacity) {
        this->Capacity=Capacity;
        Curr_Size=0;
    }
    
    int get(int key) {
        // get will search for the key in Hashmap and if not found then will return -1
        if(Hashmap.find(key)==Hashmap.end())
        return -1;
        else
        {
            // if key found then key is being accessed so it's frequency 
            // should be increased by 1 then we shift him to upper frequency LRU.
            int res = Hashmap[key]->val;
            LFU[FreqOfKey[key]].deleteNode(key);

            // count of freq will remove previous frequency and add new frequency
            this->RemoveFromCountOfFreq(FreqOfKey[key]);
            FreqOfKey[key]++;
            this->AddIntoCountOfFreq(FreqOfKey[key]);


            // insert function in LFU[freq] will return node at which it is
            // inserted after removing from previous position. 
            // as we update Hashmap as well.
            Hashmap[key] = LFU[FreqOfKey[key]].insertafterHead(key,res);
            return res;
        }
    }

   
    // function to put key with data.
    void put(int key, int value) {
       
                if(Hashmap.find(key)!=Hashmap.end())
                {
                    // deleting key as it is already present and then putting another key
                    LFU[FreqOfKey[key]].deleteNode(key);

                    // count of freq will remove previous frequency and add new frequency
                    this->RemoveFromCountOfFreq(FreqOfKey[key]);
                    FreqOfKey[key]++;
                    this->AddIntoCountOfFreq(FreqOfKey[key]);

                    Hashmap[key] = LFU[FreqOfKey[key]].insertafterHead(key,value);
                }
                else
                {
                    
                    if(Curr_Size < Capacity)
                    {
                        FreqOfKey[key] = 1;
                        CountOfFreq[FreqOfKey[key]]++;
                        Hashmap[key] = LFU[FreqOfKey[key]].insertafterHead(key,value);
                        Curr_Size++;
                    }
                    else
                    {
                        // getting minimum frequency which has some elements present in it.
                        int MinimumFreq = getMinimumCountOfKey();
                        Node* NodeTobeDeleted =  LFU[MinimumFreq].giveNodebeforeTail();
                     
                        int keydeleted = NodeTobeDeleted->key;
                        
                        LFU[MinimumFreq].deletebeforeTail();
                       
                      Hashmap.erase(keydeleted);
                      FreqOfKey.erase(keydeleted);

                    // count of freq will remove minimum frequency and add new frequency
                      this->RemoveFromCountOfFreq(MinimumFreq);
                      FreqOfKey[key] = 1;
                      this->AddIntoCountOfFreq(FreqOfKey[key]);

                    // insert function in LFU[freq] will return node at which it is
                    // inserted after removing from previous position. 
                    // as we update Hashmap as well.
                      Hashmap[key] = LFU[FreqOfKey[key]].insertafterHead(key,value);
                    }
                }
    }
// function to add count of frequency
     void AddIntoCountOfFreq(int frequency){
        CountOfFreq[frequency]++;
    }
// function to remove count of frequency and if any frequency goes down to 
// zero we remove the whole key from it
    void RemoveFromCountOfFreq(int frequency)
    {
                    CountOfFreq[frequency]--;
                    if(CountOfFreq[frequency]==0)
                    {
                        CountOfFreq.erase(frequency);
                    }
    }
    // function to return minimum element from count of frequency i.e. first element.
    int getMinimumCountOfKey(){
            auto mnFreqOfKeyitr=CountOfFreq.begin();
            return mnFreqOfKeyitr->first;
    }
    
    // function to print hashmap
    void printhash()
    {
        for(auto it:Hashmap){
            cout<<it.first<<" "<<it.second->val<<"\n";
        }
    }

    // function to print Current Cache
    void PrintCurrentCache()
    {
        for(int i=0;i<DEFAULT_SIZE;i++)
        {
            if(LFU[i].Curr_Size)
            {
                LFU[i].PrintCurrentCache();
            }
        }
    }
};



void solve(){
  

   int LFU_MAX_CACHE_SIZE;
   cout<<"ENTER THE MAXIMUM CACHE SIZE OF LFU CACHE: ";
   cin>>LFU_MAX_CACHE_SIZE;
   cout<<"\n";


   LFUCache MyLFU(LFU_MAX_CACHE_SIZE);
   
    // if you want limited queries
    cout<<"ENTER THE TOTAL NUMBER OF INTIAL QUERIES: ";
    int TOTAL_QUERIES;
    cin>>TOTAL_QUERIES;
    cout<<"\n";
//   infinite queries
//  int cnt
   while(TOTAL_QUERIES--){

    int WHICH_QUERY;
    // 1 for put and 2 for get
    cout<<"Enter 1 for PUT or 2 for GET query or -1 to stop queries: ";
    cin>>WHICH_QUERY;
    cout<<"\n";


    if(WHICH_QUERY==1)
    {
        cout<<"ENTER KEY AND VALUE FOR THE PUT QUERY: ";
        
        int Put_Key,Put_Value;
        
        cin>>Put_Key>>Put_Value;
        cout<<"\n";

        MyLFU.put(Put_Key,Put_Value);
        MyLFU.printhash();

    }
    else if(WHICH_QUERY==2)
    {
        cout<<"ENTER KEY FOR THE GET QUERY: ";
        int Get_Key;
        cin>>Get_Key;
        cout<<"\n";

        cout<<"\nValue: "<<MyLFU.get(Get_Key)<<"\n";
        
    }
    else if(WHICH_QUERY==-1)
    {
        // remaining cache:
         MyLFU.PrintCurrentCache();
         // stop recieving queries
         break;
    }
    else
    {   
        // not a valid query
        cout<<"INVALID QUERY! \n";
        continue;
    }
    // printing CURRENT LRU CACHE
    MyLFU.PrintCurrentCache();
    cout<<"\n";
   }
}


signed main() {
   
    
// init_code();
int t=1;
// cin>>t;
while(t--)
solve();

}
