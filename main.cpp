#include<bits/stdc++.h>
#define int long long
using namespace std;

string to_binary(int x,int cnt_digits){
    string b = "";
    while(x){
        b = (char)((x%2)+48) + b;
        x /= 2;
    }
    while(b.size() < cnt_digits){
        b = "0" + b;
    }
    return b;
}
string to_decimal(string bin){
        int t = 0;
        for(int i=bin.size()-1;i>=0;i--){
            t += (1ll<<(bin.size()-i-1)) * (bin[i] == '1');
        }
        stringstream ss;
        ss << t;
        return ss.str();
}
string ip_or(string ip,string ip1){
    string ip2;
    string numbers[4];
    string numbers1[4];
    int p = 0;
    int i = 0;
    while(i < ip.size()){
        if(ip[i] == '.'){
            i++;
            p++;
            continue;
        }
        numbers[p] += ip[i];
        i++;
    }
    for(int i=0;i<ip1.size();i+=8){
        numbers1[i/8] = to_decimal(ip1.substr(i,8));
    }
    for(int i=0;i<4;i++){
        int num1 = atoll(numbers1[i].c_str());
        int num2 = atoll(numbers[i].c_str());
        int num3 = num1 | num2;
        stringstream s;
        s << num3;
        ip2 += s.str();
        if(i != 3)ip2 += ".";
    }
    return ip2;
}
string sum(string ip,int cnt){
    int i = 0;
    string s = "";
    string binary = "";
    while(i < ip.size()){
        if(ip[i] == '.'){
            binary += to_binary(atoll(s.c_str()),8);
            s = "";
            i++;
            continue;
        }
        s += ip[i];
        i++;
    }
    binary += to_binary(atoll(s.c_str()),8);
    int value = atoll(to_decimal(binary).c_str())+cnt;
    binary = to_binary(value,32);
    string ans = "";
    for(int i=0;i<binary.size();i+=8){
        ans += to_decimal(binary.substr(i,8));
        ans += ".";
    }
    ans.erase(ans.size()-1,1);
    return ans;

}
class Subnet{
    public:
        string name;
        string dirRed;
        string dirDifusion;
        string mask;
        string GateWay;
        int quantity;

    public:

        void SetMaskVariable(){
            int bits = 1;
            for(bits;1<<bits<=quantity;bits++){}
            string binary;
            for(int bit=1;bit<=32;bit++){
                if((32-bit+1) <= bits)binary += "0";
                else binary += "1";
            }
            string masc = "";
            for(int i=0;i<binary.size();i += 8){
                masc += to_decimal(binary.substr(i,8));
                masc += ".";
            }
            masc.erase(masc.size()-1,1);
            mask = masc;
        }
        void SetMaskFija(int bits){
            string binary;
            for(int bit=1;bit<=32;bit++){
                if((32-bit+1) <= bits)binary += "0";
                else binary += "1";
            }
            string masc = "";
            for(int i=0;i<binary.size();i += 8){
                masc += to_decimal(binary.substr(i,8));
                masc += ".";
            }
            masc.erase(masc.size()-1,1);
            mask = masc;
        }
        Subnet(string nombre,int quantity1){
            quantity = quantity1 + 3;
            name = nombre;
        }
        void SetRed(string red){
            dirRed = red;
        }
        void SetDifusion(string difusion){
            dirDifusion = difusion;
        }
        void setGate(string gate)
        {
            GateWay = gate;
        }
        bool operator < (Subnet b) const{
            return quantity < b.quantity;
        }


};

class Network{
    public:
        vector<Subnet>Redes;
        string tipo_enmascaramiento;
        int cantidad_subredes;
        string ip_inicial;

    public :
        Network(string ip){

            cantidad_subredes = 0;
            ip_inicial = ip;
            Redes = vector<Subnet>();
        }
        bool Direccionable(){
            int p = ip_inicial.find("/");
            p++;
            string m = "";

            while(p < ip_inicial.size())m += ip_inicial[p++];
            int avaible_bits = 32 - atoi(m.c_str());
            int cnt_total = 0;

            for(auto i : Redes){
                cnt_total += i.quantity;
            }
            if(cnt_total <= 1ll<<avaible_bits){
                return 1;
            }
            else return 0;
        }
        void Set_Masks(int b){
            for(int i=0;i<Redes.size();i++){
                if(tipo_enmascaramiento == "fijo"){
                    Redes[i].SetMaskFija(b);
                }
                else {
                    Redes[i].SetMaskVariable();
                }
            }
        }
        void SetRanges(int bitsRed,int bitsHost){
            int suma = (1ll<<bitsHost)-1;
            string ip = ip_inicial.erase(ip_inicial.find("/"),ip_inicial.size()-ip_inicial.find("/")+1);
            int tot = bitsRed + bitsHost;

            for(int p=0;p<Redes.size();p++){
                string binary = "";
                for(int i=1;i<=32-tot;i++){
                    binary += "0";
                }
                binary += to_binary(p,bitsRed);
                for(int i=1;i<=bitsHost;i++){
                    binary += "0";
                }

                Redes[p].dirRed = ip_or(ip,binary);
                Redes[p].dirDifusion = sum(Redes[p].dirRed,(1<<bitsHost)-1);
            }
        }
        void SetRangesMaskVariable(){
            string ip = ip_inicial.erase(ip_inicial.find("/"),ip_inicial.size()-ip_inicial.find("/")+1);


            for(int p=0;p<Redes.size();p++){
                Redes[p].dirRed = ip;
                int bits_host=0;
                while(1ll<<bits_host < Redes[p].quantity){
                    bits_host++;
                }
                Redes[p].dirDifusion = sum(ip,(1<<bits_host)-1);
                ip = sum(Redes[p].dirDifusion,1);
            }
        }
        void SetGateways(){
            for(int p=0;p<Redes.size();p++){
                Redes[p].GateWay = sum(Redes[p].dirRed,1);
            }
        }
        void BuildNetwork(){
            sort(Redes.begin(),Redes.end());
            reverse(Redes.begin(),Redes.end());

            int bits_host = 0;
            int bits_red = 0;

            while(1<<bits_host < Redes[0].quantity){
                bits_host++;
            }
            while(1<<bits_red < cantidad_subredes){
                bits_red++;
            }

            if(!Direccionable()){
                cout << "No se puede redireccionar :(";
                return;
            }
            int p = ip_inicial.find("/");
            p++;
            string m = "";

            while(p < ip_inicial.size())m += ip_inicial[p++];
            int avaible_bits = 32 - atoi(m.c_str());
            if(avaible_bits >= bits_host+bits_red){
                tipo_enmascaramiento = "fijo";
            }
            else tipo_enmascaramiento = "variable";

            Set_Masks(bits_host);
            if(tipo_enmascaramiento == "fijo"){
                SetRanges(bits_red,bits_host);
            }
            else {
                SetRangesMaskVariable();
            }
            SetGateways();

        }
        void AgregarSubnet(Subnet x){
            Redes.push_back(x);
            cantidad_subredes++;
        }
        void printTabla(){
            string up = "Name of Subnet   |Quantity  |Mask               |Direccion Red      |Direccion Difusion      |Direccion GateWay     |";
            cout << up << '\n';
            for(auto i : Redes){
                string d = "";
                d += i.name;
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                stringstream s;
                s << i.quantity;
                d += s.str();
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                d += i.mask;
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                d += i.dirRed;
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                d += i.dirDifusion;
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                d += i.GateWay;
                while(up[d.size()] != '|'){
                    d += " ";
                }
                d += "|";
                cout << d << '\n';


            }
        }

};
main()
{

    string ip;
    cin >> ip;
    Network my_red = Network(ip);
    int cantidad_subredes;
    cin >> cantidad_subredes;

    while(cantidad_subredes--){
        cout << "Entre el nombre de la subred:" ;
        string name;
        cin >> name;
        cout << "Entre la cantidad de equipos en la subred:";
        int q;
        cin >> q;
        Subnet new_net = Subnet(name,q);
        my_red.AgregarSubnet(new_net);
    }
    my_red.BuildNetwork();

    my_red.printTabla();

}
