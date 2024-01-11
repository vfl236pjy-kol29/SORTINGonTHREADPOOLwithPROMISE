#include<iostream>
#include"OPTIMYSEDThreadPool.h"
#include<random>
#include<chrono>

using namespace std;

void taskFunc(int id, int delay)
{
	//�������� ������
	this_thread::sleep_for(chrono::microseconds(delay));
	//����� ���� � ����������
//unique_lock<mutex> l(coutLocker);
//cout << "task" << id << "made by thread_id" << this_thread::get_id << endl;

}

void quicksort(int* array, long left, long right) {
    if (left >= right) return;
    long left_bound = left;
    long right_bound = right;

    long middle = array[(left_bound + right_bound) / 2];

    do {
        while (array[left_bound] < middle) {
            left_bound++;
        }
        while (array[right_bound] > middle) {
            right_bound--;
        }

        //������ �������� �������
        if (left_bound <= right_bound) {
            std::swap(array[left_bound], array[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);
        // ��������� ��� ����� ���������
        quicksort(array, left, right_bound);
        quicksort(array, left_bound, right);
    
}

int main()
{
	srand(0);
    long arr_size = 100000000;
    int* array = new int[arr_size];
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }

	RequestHandler rh;
   
	for (int i = 0; i < 1000000000; i++)
		rh.pushRequest(taskFunc, i, 1 + rand() % 4);
	return 0;
}