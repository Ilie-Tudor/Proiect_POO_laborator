#include <iostream>
#include <cmath>
#include <conio.h>
#include <time.h>
using namespace std;

#define V_MAX 100
class Individ;

int indici_veciniI[]={-1,-1,-1, 0,0, 1,1,1};
int indici_veciniJ[]={-1, 0, 1,-1,1,-1,0,1};

char randTip(){
    int tip = rand()%2;
    char specie;
    if(tip==0){
        specie = 'o';
    }else{
        specie = '+';
    }
    return specie;
}
char tabela_temp[20][70];

void GenerareTabela();
int verifIndici(int x, int y);
double getEnergieDisp(int i, int j);
int getVeciniLiberi(int i, int j);
void actualizareAportEnergetic();
void actualizareNasteri(int i, int j, Individ &x);

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
    friend class Individ;
    friend int verifIndici(int x,int y);
    friend double getEnergieDisp(int i, int j);
    friend void actualizareAportEnergetic();
    friend void actualizareIndivizi();
    friend void actualizareNasteri();
    friend void AfisareTabela();
    Individ tabela[20][70];
    friend void GenerareTabela();
}T;
void Individ::hraneste(){
    m_energie += getEnergieDisp(m_x,m_y)/(abs(V_MAX/2-m_varsta)/4.0+25);
}
void Individ::ataca(){
    for(int k=0;k<8;k++){
        if(verifIndici(m_x+indici_veciniI[k],m_y+indici_veciniJ[k])==-1 && T.tabela[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]].getTip()!=m_specie){
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
        if(verifIndici(m_x+indici_veciniI[k],m_y+indici_veciniJ[k])==1 && tabela_temp[m_x+indici_veciniI[k]][m_y+indici_veciniJ[k]]=='_'){
            I[n]=m_x+indici_veciniI[k];
            J[n]=m_y+indici_veciniJ[k];
            n++;
        }
    }
    for(int i=0;i<n;i++){
        int x = rand();
        if(x%4==0){
                //cout<<(1+(int)(abs(V_MAX/2-m_varsta)/(m_energie+25)))<<" ";
            if(m_energie>CostEnergie+10){
                m_energie-=CostEnergie;
                tabela_temp[I[i]][J[i]] = m_specie;
            }
            // aici trebuie sa mai fac formula pentru nasteri
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
    m_energie -= 1; // trebuie vazut daca 1 e o valoare buna
    if(m_varsta>=V_MAX || m_energie<=0){
        moare();
    }
}

// aici se termina functiile din clasa Individ

int verifIndici(int x,int y){
    if(x<0 || x>=20 || y<0 || y>=70){
        return 0; // inseamna pozitie in afara tabelei
    }
    else if(T.tabela[x][y].esteViu()){
        return -1; // inseamna ca pozitia e ocupata de un individ
    }
    else return 1; // inseamna ca pozitia e libara si valida
}

int getVeciniLiberi(int i, int j){
    int rez=0;
    for(int k=0;k<8;k++){
        if(verifIndici(i+indici_veciniI[k],j+indici_veciniJ[k])==1){
            rez++;
        }
    }
    return rez;
}
double getEnergieDisp(int i, int j){
        double rez=0.0;
        for(int k=0;k<8;k++){
            if(verifIndici(i+indici_veciniI[k],j+indici_veciniJ[k])==1){
                rez+=T.tabela[i+indici_veciniI[k]][j+indici_veciniJ[k]].getEnergie();
            }
        }
        return rez;
}
void actualizareAportEnergetic(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            int rez=getVeciniLiberi(i,j);
            if(verifIndici(i,j)==1) T.tabela[i][j].setEnergie(rez);
        }
    }
}
void actualizareIndivizi(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            T.tabela[i][j].actualizeaza();
        }
    }
}
void actualizareNasteri(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            if(tabela_temp[i][j]=='+'){
                Individ nou(i,j,'+');
                T.tabela[i][j] = nou;
            }
            else if(tabela_temp[i][j]=='o'){
                Individ nou(i,j,'o');
                T.tabela[i][j] = nou;
            }
        }
    }
}
void ResetareTabelaTemp(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            tabela_temp[i][j]='_';
        }

    }

}
void AfisareTabela(){
    for(int i=0;i<20;i++){
        for(int j=0;j<70;j++){
            cout<<T.tabela[i][j].getTip()<<" "; //aici pot afisa diverse despre tabela si indivizi
        }
        cout<<endl;
    }
    cout<<endl;

}
void GenerareTabela(){// de generat tabela aleator... deocamdata sunt numai niste pozitii fixe
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
        T.tabela[X[i]][Y[i]]=init;
    }
    actualizareAportEnergetic();
//    actualizareIndivizi();
//    actualizareNasteri();
//    ResetareTabelaTemp();
}
char ch;
void Frame(){
    srand (time(NULL));
    ch = _getch();
    actualizareAportEnergetic();
    actualizareIndivizi();
    actualizareNasteri();
    ResetareTabelaTemp();
    system("cls");
    AfisareTabela();
}

int main(){
    GenerareTabela();
    AfisareTabela();

while(ch!='a'){
    Frame();
}

}

