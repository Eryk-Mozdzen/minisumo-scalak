#include <iostream>
#include <vector>

using namespace std;

void smieszek(int tab[], int n) {
  for(int i=0; i<n; i++)
    cout << tab[i] << " ";
  cout << endl;
}

void smieszek(vector<int> tab, int n) {
  for(int i=0; i<n; i++)
    cout << tab[i] << " ";
  cout << endl;
}

int main() {

    int arr[] = {2,3,4,5};
    vector<int> vec = {2,3,4,5};

    smieszek(arr,4);
    smieszek(vec,4);
    smieszek({2,3,4,5},4);

    return 0;
}
