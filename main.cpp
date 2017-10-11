#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include<vector>
#include <sstream>
#include <string>
#include<algorithm>
#include<map>
#include <bits/stdc++.h>

using namespace std;
struct node
{
    int data;
    string s;
    int label;
int threshold;
    node *left,*right;
    int positive,negative;
};
class decision_tree
{
private:
    node *root=NULL;
public:
    double entropy(int pos,int neg)
    {
        if(pos==0 && neg==0)
        {
            return 0;
        }
    double x=pos/((double)pos+neg);
    double y=neg/((double)pos+neg);
    if(pos==neg){
        return 1;
    }
    else if(pos==0 || neg==0){
        return 0;
    }
    else{
        return -(x*log2(x)+y*log2(y));
    }
    }
    int best_split(vector<pair<int,int> > word_label,double &max_IG)
    {
        int C=0;
       int TP=0,TN=0,pos=0,neg=0;
       //cout<<"before"
        sort(word_label.begin(),word_label.end());//,[](pair<int,int> i,pair<int,int> j){return i.first<j.first;});
        for(int i=0;i<word_label.size();i++){
            if(word_label[i].second==1)
                TP++;
            else
                TN++;
        }
        if(TP+TN==0)
        {
            return 0;
        }
        double TE=entropy(TP,TN),right_Ent,left_Ent,total;
	max_IG=-1;
        for(int i=0;i<word_label.size()-1;i++){
            if(word_label[i].second==1)
                pos++;
            else
                neg++;
            if(word_label[i].first!=word_label[i+1].first){
                left_Ent=entropy(pos,neg);
                right_Ent=entropy(TP-pos,TN-neg);
		total=0;
		total+=(left_Ent*(pos+neg))/(TP+TN);
		total+=(right_Ent*(TP+TN-pos-neg))/(TP+TN);
                if(max_IG<TE-total){
                    max_IG=TE-total;
                    C=word_label[i].first;
                }
            }
        }
        return C;
    }
    node * ID3(vector<pair <int,map<int,int> > > v,vector<pair<int,string> > words,node *root)
    {
vector<pair <int,map<int,int> > > v_left;
vector<pair <int,map<int,int> > > v_right;
   vector<pair <int,int> > word_label;
   int i,j;
   double max_inf=-1;
   int w_index;
   string str;
   int split;
   int w;
   for(i=0;i<words.size();i++)
   {
       for(j=0;j<v.size();j++)
       {
          if(v[j].second.find(words[i].first)!=v[j].second.end()){
                word_label.push_back(make_pair(v[j].second[words[i].first],v[j].first));
          }else{
                word_label.push_back(make_pair(0,v[j].first));
          }

       }
	double i_gain;
	int c=best_split(word_label,i_gain);
word_label.clear();
       if(i_gain>max_inf)
       {
           max_inf=i_gain;
           w_index=words[i].first;
           str=words[i].second;
           w=i;//words[i].second;
           split=c;
       }
   }
int pos=0,neg=0;

   for(i=0;i<v.size();i++)
   {
       if(v[i].first ==1)
       {
           pos++;
       }
       else if(v[i].first==-1)
       {
           neg++;
       }
       if(v[i].second.find(w_index)!=v[i].second.end())
       {
           if(v[i].second[w_index] <split)
           {
            v_left.push_back(make_pair(v[i].first,v[i].second));
           }
           else if(v[i].second[w_index]>split)
           {
            v_right.push_back(make_pair(v[i].first,v[i].second));
           }
       }
       else
       {
         v_left.push_back(make_pair(v[i].first,v[i].second));
       }
   }
   if( words.size()==0 || max_inf<0 || max_inf>3)
{
    root->label=(pos>=neg)?1:-1;
}
 else if(pos==0)
{
    root->label=-1;
}
else if(neg==0)
{
    root->label=1;
}
else{
   words.erase(words.begin()+w);
        root->label=0;
        root->positive=pos;
        root->negative=neg;
root->data=w_index;
root->s=str;
root->threshold=split;
root->left=new node;
cout<<root->s<<endl;
root->left=ID3(v_left,words,root->left);
root->right=new node;
root->right=ID3(v_right,words,root->right);
}
return root;
}
double test_data(vector<pair<int,map<int,int> > > test,node *root)
{
int i;
node *q;
q=root;
int count=0;
for(i=0;i<test.size();i++)
{
root=q;
int x=test[i].first;
while(root->label==0){
if(test[i].second.find(root->data)!=test[i].second.end())
{
if(test[i].second[root->data]<root->threshold)
{
root=root->left;
}
else
{
root=root->right;}
}
else
{
root=root->left;
}
if(root->label==x)
{
count++;
}
}
}
return count;
}
vector<pair<int,map<int,int> > > sampling(vector <string> st)
{
vector<pair<int,map<int,int> > > test;
    for(int i=0;i<st.size();i++)
    {
        int count=0;
    for(int j=0;j<st[i].size();j++)
   {
     if(st[i][j]==':')
     {
         st[i][j]=' ';
         count++;
     }
   }
   int x,num1,num2;
   num1=1,num2=2;
   stringstream geek(st[i]);
   map <int,int> m;
   geek>>x;
   if(x>=7){
    x=1;
   }
   else if(x<=4)
    {x=-1;}

   int k=0;
   while(k!=(count+1))
   {
       geek>>num1>>num2;

       m.insert(pair<int,int>(num1,num2));
       k++;
   }
   test.push_back(make_pair(x,m));
    }
   return test;
}
void pruning(vector<pair<int,map<int,int> > > test,node *head,node *rot)
{
    if(head->left->label==0)
    pruning(test,head->left,rot);
    if(head->right->label==0)
    pruning(test,head->right,rot);
    double prun_before=test_data(test,rot);
if(head->positive>=head->negative)
{
    head->label=1;
}
else
{
    head->label=-1;
}
double prun_after=test_data(test,rot);
if(prun_after<prun_before)
{
    head->label=0;
}
else
{
double x=prun_after/test.size();
x=x*100;
cout<<head->s<<"  "<<x<<endl;
}
}
double noise(vector<pair<int,map<int,int> > > test,node *head,double nois)
{
vector<pair<int,map<int,int> > > tr;
tr=test;
random_shuffle(test.begin(),test.end());
    int i,j,count;
    vector <int> X;
    int n_value=(nois/100)*test.size();
  /* for(i=0;i<n_value;i++)
    {	//srand(time(NULL));
        int x=rand()%test.size();
        for(j=0;j<X.size();j++)
        {
          if(x==X.at(j))
            count++;
        }
        if(count==0)
        {
            X.push_back(x);
        }
    }*/
//cout<<X.size()<<endl;
    for(i=0;i<n_value;i++)
    {
     if(test[i].first==1)
     {
        test[i].first=-1;
     }
     else
     {
         test[i].first=1;
     }
    }
    double n_acc=test_data(test,head);
    return n_acc;
}
int get_label(map<int,int> m,node * hed)
{
    while(hed->label==0){
if(m.find(hed->data)!=m.end())
{
if(m[hed->data]<hed->threshold)
{
hed=hed->left;
}
else
{
hed=hed->right;}
}
else
{
hed=hed->left;
}
}
return hed->label;

}
double random_forests(vector<pair<int,string> > word_attr,vector<pair<int,map<int,int> > > test_data,int siz,int n_tree)
{

    vector<pair<int,string> >sampl;
    vector <node* > nud;
  //  map<int,string> sam;
int i,j,k,t;
for(k=0;k<n_tree;k++){
        random_shuffle(word_attr.begin(),word_attr.end());
                i=1;
    while(i!=siz)
    {
        int x=rand()%word_attr.size();
        string s=word_attr[x].second;
        int y=word_attr[x].first;
      /*  for(j=0;j<sampl.size();j++)
        {
            if(y!=sampl[j].first)
            {
                sampl.push_back(make_pair(y,s));
                i++;
            }
        }*/
        sampl.push_back(make_pair(y,s));
        i++;
    }
            node *that=new node;
    that=ID3(test_data,sampl,that);
    nud.push_back(that);

}
vector <int> final_lab;int count=0;
for(i=0;i<test_data.size();i++){
 int pos=0,neg=0;
 for(j=0;j<nud.size();j++)
 {
     int x;
     x=get_label(test_data[i].second,nud.at(j));
     if(x==1)
        pos++;
     else
        neg++;
 }
 if(pos>=neg)
    {
        //cout<<"pos"<<endl;
     if(test_data[i].first==1){
            count++;
            //cout<<count<<endl;
     }
    }
 else if(pos<neg)
 {
    // cout<<"neg"<<endl;
          if(test_data[i].first==-1){
                count++;
          //cout<<count<<endl;
          }
 }
}
//double ans=(count/test_data.size());
return count;
    }

};
int main(int argc,char **argv)
{
       decision_tree a;
ifstream input,in;
vector <string> str;
//s.open("output.txt");
input.open("dataset.feat");
in.open("imdb.vocab");
while(true)
{
    if(!input.good())
    {
        break;
    }
    string line;
    while(getline(input,line))
    {
      str.push_back(line);
    }
}
    vector<pair<int,map<int,int> > > train;
    train=a.sampling(str);
    vector <pair<int,string> > words;
    int cnt=0;
    while(true)
    {
        if(!in.good())
        {
            break;
        }
        string line;
        while(getline(in,line))
        {
            words.push_back(make_pair(cnt,line));
            cnt++;
        }
    }
    vector<pair<int,string> > random_word;
vector<pair<double,int> > polar_word;
ifstream t;
t.open("imdbEr.txt");
int z=0;
while(true)
{
if(!t.good())
{
break;
}
double d;
t>>d;
polar_word.push_back(make_pair(d,z));
z++;
}
sort(polar_word.begin(),polar_word.end());//,[](pair<int,double> i,pair<int,double> j){return i.first<j.first;});
int f;
for(int i=0;i<polar_word.size();i++)
{
if(polar_word[i].first>0)
{
f=i;
break;
}
}
for(int i=0;i<2500;i++)
{
random_word.push_back(make_pair(polar_word[i].second,words[polar_word[i].second].second));
}
for(int i=f;i<(f+2500);i++)
{
random_word.push_back(make_pair(polar_word[i].second,words[polar_word[i].second].second));
}
ifstream inp;
inp.open("dataset_test.feat");
vector<pair<int,map<int,int> > > test;
vector <string> st;
while(true)
{
if(!inp.good())
{
break;
}
    string line;
    while(getline(inp,line))
    {
      st.push_back(line);
    }
}
ifstream wooh;
wooh.open("dataset_test.feat");
vector<pair<int,map<int,int> > > valid_test;
vector <string> strng;
while(true)
{
if(!wooh.good())
{
break;
}
    string line;
    while(getline(wooh,line))
    {
      strng.push_back(line);
    }
}
valid_test=a.sampling(strng);
   test=a.sampling(st);
   node *root=new node;
if(strcmp(argv[1],"exp1")==0)
{
cout<<"dataset for train is created and stored in dataset.feat"<<endl;
cout<<"dataset for test is created and stored in dataset_test.feat"<<endl;
}
else if(strcmp(argv[1],"exp2")==0)
{
cout<<"decision tree trained for 5000 attributes"<<endl;
cout<<"attributes used for training"<<endl;
root=a.ID3(train,random_word,root);
double acc=a.test_data(train,root);
double acc1=a.test_data(test,root);
double g=(acc/test.size());
double g1=(acc1/test.size());
g=g*100;
g1=g1*100;
cout<<"accuracy="<<g<<" percentage-training"<<endl;
cout<<"accuracy="<<g1<<" percentage-test"<<endl;

}
else if(strcmp(argv[1],"exp3")==0)
{
cout<<"attributes used for training"<<endl;
root=a.ID3(train,random_word,root);
int nois;
cout<<"enter the percentage of noise you want to enter...for eg if you want to 1% noise enter 1"<<endl;
cin>>nois;
double p=a.noise(test,root,nois);
double f=(p/test.size());
f=f*100;
cout<<"accuracy="<<f<<" percentage after adding "<<nois<<" percentage of noise"<<endl;
}
else if(strcmp(argv[1],"exp4")==0)
{
cout<<"attributes used for training"<<endl;
root=a.ID3(train,random_word,root);
cout<<"tree is pruned for some validation data generated randomly using test data"<<endl;
a.pruning(valid_test,root,root);
double ac=a.test_data(test,root);
double g=(ac/test.size());
g=g*100;
cout<<"accuracy="<<g<<" percentage after pruning"<<endl;
}
else if(strcmp(argv[1],"exp5")==0)
{
cout<<"enter the number of trees to be made in decision forests"<<endl;
int q;
cin>>q;
double y;
y=a.random_forests(random_word,test,100,q);
double g=(y/test.size());
g=g*100;
cout<<"accuracy="<<g<<" percentage for "<<q<<" trees in forest"<<endl;
}
    return 0;
}


