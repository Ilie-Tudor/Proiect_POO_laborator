#include <iostream>
#include <cmath>
#include <conio.h>
#include <time.h>
using namespace std;


const int V_MAX = 100;
class Individ;

int indici_veciniI[]={-1,-1,-1, 0,0, 1,1,1};
int indici_veciniJ[]={-1, 0, 1,-1,1,-1,0,1};

class Individ{
    int m_x,m_y;
    char m_specie;
    int m_varsta;
    double m_energie;
    unsigned char m_verif_viu;
public:
    Individ(){
        m_x = 0;
        m_y = 0;
        m_specie = '_';
        m_varsta = 0;
        m_energie = 0;
        m_verif_viu = '0';
    }
    Individ(int x, int y, char tip){
        m_x = x;
        m_y = y;
        m_specie = tip;
        m_varsta = 0;
        m_energie = 10; // 10 am ales sa fie energia initiala;
        m_verif_viu = '1';
    }
    Individ(Individ &c){
        m_x=c.m_x;
        m_y=c.m_y;
        m_specie=c.m_specie;
        m_varsta=c.m_varsta;
        m_energie=c.m_energie;
        m_verif_viu=c.m_verif_viu;
    }
    Individ operator=(const Individ &c){
        m_x=c.m_x;
        m_y=c.m_y;
        m_specie=c.m_specie;
        m_varsta=c.m_varsta;
        m_energie=c.m_energie;
        m_verif_viu=c.m_verif_viu;
        return *this;
    }
private:
    void hraneste();
    void inmulteste();
    void ataca();
    void moare();
    void imbatraneste();
public:
    void actualizeaza(){
        if(m_verif_viu=='1'){
            hraneste();
            inmulteste();
            ataca();
            imbatraneste();
        }
    }
    void die(){
        if(m_verif_viu=='1')    moare();
    }
//  getteri
    short int esteViu(){
        return m_verif_viu=='1' ? 1:0 ;
    }
    char getTip(){
        return m_specie;
    }
    double getEnergie(){
        return m_energie;
    }
    int getVarsta(){
        return m_varsta;
    }
//  setteri
    void setEnergie(double x){
        m_energie=x;
    }
};
class Tabela{
    Individ tabela[20][70];
    friend class Individ; // am declarat clasa Individ ca fiind prietena pentru Tabela pentru a avea acces la metodele pentru managementul tabelei
    char tabela_temp[20][70]; // tabela temporala este folosita ca nasterile sa fie facute simultan, in acelasi frame al jocului. Altfel, copii se nasc pe rand si pot fi atacati imediat.
    void GenerareTabela();
    double getEnergieDisp(int i, int j); // aceasta functie returneaza energia disponibila pentru individ, luand in calcul aportul energetic al pozitiilor adiacente
    void actualizareAportEnergetic();
    void actualizareIndivizi();
    void actualizareNasteri(); // se actualizeaza nasterile stocate in tabela temporala
    void AfisareTabela();
    void ResetareTabelaTemp();
    int getVeciniLiberi(int i, int j); // functia returneaza numarul de vecini liberi pe care ii are o pozitie libera, acesta reprezentand aportul energetic
    char randTip(); // aceasta functie imi returneaza random una din cele 2 specii
    int verifIndici(int x, int y); // ferifica daca pozitia (x,y) se afla in tabela si este libera sau ocupata
    bool gameOver();
public:
    void Init(){// initializarea tabelei
            GenerareTabela();
            AfisareTabela();
            cout<<"Tineti apasata o tasta oarecare inafara de tasta a ca jocul sa continue \nPentru a termina jocul apasati tasta a";
    }
    void ClientInit(int vi[], int vj[], char vs[], int n){

        for(int k=0; k<n; k++){
            Individ nou(vi[k],vj[k],vs[k]);
            tabela[vi[k]][vj[k]] = nou;
        }
        AfisareTabela();
        cout<<"Tineti apasata o tasta oarecare inafara de tasta a ca jocul sa continue \nPentru a termina jocul apasati tasta a";

    }
    void Frame(){// un pas in loop-ul programului
        actualizareAportEnergetic();
        actualizareIndivizi();
        actualizareNasteri();
        ResetareTabelaTemp();
        system("cls");
        AfisareTabela();
    }
}T;
void Individ::hraneste(){
    m_energie += T.getEnergieDisp(m_x,m_y)/(abs(V_MAX/2-m_varsta)/4.0+25);
}
void Individ::ataca(){
    for(int k=0;k<8;k++){
        if(T.verifIndici(m_x+indici_veciniI[k],m_y+indici_veciniJ[k])==-1 && T.tabela[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]].getTip()!=m_specie){
            double e=T.tabela[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]].getEnergie();
            if(e<m_energie){
                T.tabela[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]].die();
                m_energie-=e;
            }
        }
    }

}
void Individ::inmulteste(){
    int CostEnergie=5;
    int I[8];
    int J[8];   //vectorii retin indicii pozitiiolr libere unde se pot naste indivizi
    int n=0;
    for(int k=0;k<8;k++){
        if(T.verifIndici(m_x+indici_veciniI[k],m_y+indici_veciniJ[k])==1 && T.tabela_temp[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]]=='_'){
            I[n]=m_x+indici_veciniI[k];
            J[n]=m_y+indici_veciniJ[k];
            n++;
        }
    }
    for(int i=0;i<n;i++){
        int x = rand();
        if(x%(3+(int)(abs(V_MAX/2-m_varsta)/(m_energie+25)))==0){
            if(m_energie>CostEnergie+10){
                m_energie-=CostEnergie;
                T.tabela_temp[I[i]][J[i]] = m_specie;
            }
        }
    }
}
void Individ::moare(){
    m_specie='_';
    m_varsta = 0;
    m_energie = 0;
    m_verif_viu = '0';
}
void Individ::imbatraneste(){
    m_varsta += 1;
    m_energie -= 1;
    if(m_varsta>=V_MAX || m_energie<=0){
        moare();
    }
}
// aici se termina functiile din clasa Individ

char Tabela::randTip(){ // aceasta funtie imi returneaza random una din cele 2 specii
    int tip = rand()%2;
    char specie;
    if(tip==0){
        specie = 'o';
    }else{
        specie = '+';
    }
    return specie;
}
void Tabela::ResetareTabelaTemp(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            tabela_temp[i][j]='_';
        }

    }
}
int Tabela::verifIndici(int x,int y){
    if(x<0 || x>=20 || y<0 || y>=70){
        return 0; // inseamna pozitie in afara tabelei
    }
    else if(tabela[x][y].esteViu()){
        return -1; // inseamna ca pozitia e ocupata de un individ
    }
    else return 1; // inseamna ca pozitia e libara si valida
}
int Tabela::getVeciniLiberi(int i, int j){
    int rez=0;
    for(int k=0;k<8;k++){
        if(verifIndici(i+indici_veciniI[k],j+indici_veciniJ[k])==1){
            rez++;
        }
    }
    return rez;
}
double Tabela::getEnergieDisp(int i, int j){
        double rez=0.0;
        for(int k=0;k<8;k++){
            if(verifIndici(i+indici_veciniI[k],j+indici_veciniJ[k])==1){
                rez+=tabela[i+indici_veciniI[k]][j+indici_veciniJ[k]].getEnergie();
            }
        }
        return rez;
}
void Tabela::actualizareAportEnergetic(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            int rez=getVeciniLiberi(i,j);
            if(verifIndici(i,j)==1) tabela[i][j].setEnergie(rez);
        }
    }
}
void Tabela::actualizareIndivizi(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            tabela[i][j].actualizeaza();
        }
    }
}
void Tabela::actualizareNasteri(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            if(tabela_temp[i][j]=='+'){
                Individ nou(i,j,'+');
                tabela[i][j] = nou;
            }
            else if(tabela_temp[i][j]=='o'){
                Individ nou(i,j,'o');
                tabela[i][j] = nou;
            }
        }
    }
}
void Tabela::AfisareTabela(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            cout<<tabela[i][j].getTip()<<" "; //aici pot afisa diverse despre tabela si indivizi
        }
        cout<<endl;
    }
    cout<<endl;

}
void Tabela::GenerareTabela(){
    ResetareTabelaTemp();
    int n = 0;
    int X[1400];
    int Y[1400];
    char S[1400];
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            int x = rand()%10;
            if(x==0){
                X[n]=i;
                Y[n]=j;
                S[n] = randTip();
                n++;
            }
        }
    }
    for(int i=0;i<n;i++){
        Individ init(X[i],Y[i],S[i]);
        tabela[X[i]][Y[i]]=init;
    }
    actualizareAportEnergetic();
}

bool Tabela::gameOver(){ //aici am definit functia care verifica daca mai exista indivizi pe tabela, dar nu am folosit-o pentru ca am ales valorile si formulele in asa fel incat acest lucru sa nu se intample
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            if(tabela[i][j].esteViu()==1){
                return false;
            }
        }
    }
    return true;
}
//  aici se termina functiile din clasa Tabela

char ch;
int main(){
    srand (time(NULL));

    int mode;
    cout<<"Selectati modul in care sa se initializeze tabela:"<<"\n"<<"1 = Tabela generata aleatoriu\n2 = Tabela generata de user\n";
    cin>>mode;
    if(mode==1){
        T.Init();
        while(ch!='a'){ // la fiecare pas trebuie apasat pe orice tasta cu exceptia lui "a" ca jocul sa continue. Daca se apasa "a", jocul se termina.
            ch = _getch();
            T.Frame();
        }
    }
    else if(mode==2){
        cout<< "Cati indivizi?\n";
        int n;
        cin>>n;
        cout<< "Afisati tuplurile pentru indivizi\n";
        int I[n];
        int J[n];
        char SP[n];
        for(int i=0;i<n;i++){
            cin>>I[i]>>J[i]>>SP[i];
        }
        T.ClientInit(I,J,SP,n);
        while(ch!='a'){ // la fiecare pas trebuie apasat pe orice tasta cu exceptia lui "a" ca jocul sa continue. Daca se apasa "a", jocul se termina.
            ch = _getch();
            T.Frame();
        }

    }
    return 0;
}

