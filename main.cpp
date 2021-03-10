#include <iostream>
#include <cmath>
#include <conio.h>
#include <time.h>
using namespace std;

#define V_MAX 100
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
    friend class Individ;
    char tabela_temp[20][70];
    void GenerareTabela();
    double getEnergieDisp(int i, int j);
    void actualizareAportEnergetic();
    void actualizareIndivizi();
    void actualizareNasteri();
    void AfisareTabela();
    void ResetareTabelaTemp();
    int getVeciniLiberi(int i, int j);
    char randTip(); // aceasta functie imi returneaza random una din cele 2 specii
    int verifIndici(int x, int y);
public:
    void Init(){// initializarea tabelei
            GenerareTabela();
            AfisareTabela();
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
        if(x%4==0){
                //cout<<(1+(int)(abs(V_MAX/2-m_varsta)/(m_energie+25)))<<" ";
                // aici trebuie sa mai fac formula pentru nasteri care sa includa V_MAX/2 dar pana acuma nu mi-a placut ce formule am creeat
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
//  aici se termina functiile din clasa Tabela

char ch;
int main(){
    srand (time(NULL));
    T.Init();
    while(ch!='a'){ // la fiecare pas trebuie apasat pe orice tasta cu exceptia lui "a" ca jocul sa continue. Daca se apasa "a", jocul se termina.
        ch = _getch();
        T.Frame();
    }
    return 0;
}

