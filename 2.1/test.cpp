#include<bits/stdc++.h>
#include<Eigen/Dense>

using namespace Eigen;

int main(){
    MatrixXd m(2,2);
    m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);
  std::cout << m << std::endl;
  VectorXd v(9);
  v << 1,2,3,4,5,6,7,8,9;
  VectorXd v1(9);
  v1 << 2,4,6,8,10,12,14,16,18;
  std::cout << v1 << std ::endl;
  std::cout << v << std ::endl;
  std::cout<< v.cwiseQuotient(v1) <<std::endl;
  

}
