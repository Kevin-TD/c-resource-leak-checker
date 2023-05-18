int method1(int a, int b) {
  return a + b; 
}

int method2(int a, int b) {
  return a - b; 
}

int method3(int a) {
  return a;
}

int main() {
  int a = 5;
  int b = 6; 
  int c = getchar(); 

  if (c > 15) {
    a = method1(a, a);
  }
  else {
    a = method2(a, b); 
  }
  b = method2(method3(c), method3(a - c)); 


  return 0;
}
