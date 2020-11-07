#include <iostream>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <fstream>

using namespace std;

typedef const char* filename; //tipuskonverzio, ahol jelezve van, hogy a vart string egy fajlnak a neve
vector<int> freq;
int num_o_q;

bool isLanguageSelected = false;
string Language = "";

void freq_initial(filename fname)
{
    int linenum=0;
    char c;
    ifstream is;
    is.open(fname);
    while(is.get(c))
        if(c=='\n')
           ++linenum;
    is.close();
    for(int i=0; i<linenum; i++)
        freq.push_back(1);
    num_o_q=freq.size();
}


string ngetline(filename fname, string& s, int line)
{
    ifstream is;
    is.open(fname);
    int i;
    for(i=0; line>=0 && i<line; i++)
        getline(is, s);
    is.close();
    return s;
}

struct question{
    string quest;
    string answ;
    int num;
};


question make_question(filename fname)
{
    // making choice
    int sum=0;
    for(int i=0; i<num_o_q; i++)
        sum+=freq[i];

    int quest = rand() % sum + 1;
    int choice=0;
    for(int s=0; choice<num_o_q && s<quest; choice++)
       s+=freq[choice];
    // making of question
    string s;
    ngetline(fname, s, choice);
    string left, right;

    left.assign(s, 0, s.find('='));
    left.append(" (EN)");
    right.assign(s, s.find_first_of('=')+1, s.size()- s.find_first_of('=')-1);
    right.append(" (HU)");

    question q;
    q.num = choice;
    int v;
    if(!isLanguageSelected)
        v=rand() % 2;
    else if (Language == "EN")
        v=0;
    else if (Language == "HU")
        v=1;
    if(v==0)
        {q.quest=left; q.answ=right; }
    else
        {q.quest=right; q.answ=left; }
    return q;
}

void testing(int mode, filename fname)
{
    //Initialization
    int n;
    if(mode == 1 )
        n = (num_o_q<40) ? num_o_q-1 : 40;
    else if(mode == 2)
        n = num_o_q;
    else
        n = mode - 2;
    int last[n][2];
    int known = 0;
    for(int i=0; i<n; i++){
        last[i][0] = 0;
        last[i][1] = 0;}
    question q;
    int d=1;
    do{
        // Choose question
        do{
            bool ok=true;
            q=make_question(fname);
            for(int i=0; i<n; i++)
                if(last[i][0] == q.num)
                    ok=false;
            if(ok) break;
        } while(true) ;

        // Ask question
        cout<<"\n"<<d<<" "<<"("<< known<<", "<<n<<")\t"<<q.quest<<" = ";
        char c;
        getchar();
        do
            c=getchar();
        while(c!='\n');
        cout<<"\t\t\t"<<q.answ<<endl;
        cout<<"\n\tTudtad? (y-yes, n-no, r-repeat, q-quit)? ";
        // Shift over
        for(int i=1; i<n; i++){
            last[i-1][0] = last[i][0];
            last[i-1][1] = last[i][1];}
        last[n-1][0]=q.num;
        last[n-1][1]=0;

        //Controll
        do
            cin>>c;
        while(!(c=='q' || c=='Q' || c=='y' || c=='Y' || c=='r' || c=='R' || c=='n' || c=='N'));
        if(c=='q' || c=='Q') break;
        else if(c=='y' || c=='Y')
        {
            last[n-1][1] = 1;
            if(freq[q.num - 1] > 1)
                freq[q.num - 1]-=1;
        }
        else if(c=='r' || c=='R') continue;
        else if(c=='n' || c=='N') freq[q.num-1]+=1;
        known = 0;
        for(int i=0; i<n; i++)
            known+=last[i][1];
        d++;
    }while( mode == 1 || d<=n ) ;
    cout<<"("<<known<<", "<<n<<")"<<endl;
}

int main(int argc, char* args[])
{
    bool isModSelected = false;
    srand(time(0));
    // CONTROLL
    if(argc<2){
        cerr<<"\tKerlek, nevezd meg a fajlt, ahol a szavak talalhatok"<<endl;
        exit(EXIT_SUCCESS);
        }
    if (argc == 3)
    {
        if( args[2][0] != '1' && args[2][0] != '2' && args[2][0] != '3' )
        {
            cerr<<"\tHibas argumentum"<<endl;
            exit(EXIT_SUCCESS);
        }
        else
            isModSelected = true;
    }
    if (argc == 4)
    {
        if( args[2][0] != '1' && args[2][0] != '2' && args[2][0] != '3' )
        {
            cerr<<"\tHibas argumentum"<<endl;
            exit(EXIT_SUCCESS);
        }
        else
            isModSelected = true;
        if( args[3] == "HU" || args[3] == "EN")
        {
            cerr<<"\tHibas argumentum"<<endl;
            exit(EXIT_SUCCESS);
        }
        else
        {
            isLanguageSelected = true;
            Language = args[3];
        }
    }
    if (argc > 4)
    {
        cerr<<"Ismeretlen argumentumok"<<endl;
        exit(EXIT_SUCCESS);
    }
    filename fname=args[1];
    ifstream fp;
    fp.open(fname);
    if(fp == NULL){
        cerr<<"\tA fajl nem letezik"<<endl;
        exit(EXIT_SUCCESS);
    }
    {
        string s;
        for(int i=0; (i<num_o_q || num_o_q==0) && getline(fp, s); i++){
            if(s.find('=') >= s.size()-1){
                cerr<<"\tEz az allomany nem tartalmaz megfelelo szoveget a kikerdezeshez"<<endl;
                cerr<<"\tBaris hibat talaltam a(z) "<<i+1<<" sornal"<<endl;
                exit(EXIT_SUCCESS);
            }}
    }
    fp.close();


    freq_initial(fname);
    cout<<endl;
    cout<<"\tEz egy szokerdezo program, amely segit neked tesztelni az angol tudasodat" <<endl;
    cout<<"Szavakat valaszt ki az altalad megadott alomanybol, es egymas utan kerdezgeti toled" <<endl;
    cout<<endl;
    char c;
    do{
        cout<<"Osszesen "<<num_o_q<<" szo van" <<endl;
        if(!isModSelected)
        {
            cout<<"Lassuk, mennyire vagy okos"<<endl;
            cout<<"\tValasztd ki a kikerdezes modjat: "<<endl;
            cout<<"\t(1) Folyamatos kikerdezes\n";
            cout<<"\t(2) Mindent egyszer\n";
            cout<<"\t(3) Villamkerdesek\n";
            cout<<"\t(Q/q) Kilepes \n";
            cout<<"\n\t";
            cin>>c;
            }
        else c = args[2][0];
        switch(c){
            case '1': {
                testing(1, fname);
                if(isModSelected)
                    c = 'q';
                break;
                }
            case '2': {
                testing(2, fname);
                if(isModSelected)
                    c = 'q';
                break;
                }
            case '3':{
                int num;
                do{
                    cout<<"\tHany kerdest szeretnel? ";
                    cin>>num;
                }while(num > num_o_q);
                testing(num+2, fname);
                if(isModSelected)
                    c = 'q';
                break;
            }}}
    while(c!='q' && c!='Q') ;
    return 0;
}
