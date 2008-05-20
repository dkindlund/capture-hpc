// Contents of file "calc.h": 
//gsoap ns service name: calculator
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service port: http://192.168.0.131:1234
//gsoap ns service namespace: calculator
int ns__add(int a, int b, int &result); 
int ns__test(char * a, char ** result); 
int ns__sub(double a, double b, double &result); 
