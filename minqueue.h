/*minqueue.h*/

//
// Min queue that stores (key, value) pairs using a min-heap 
// implementation.  When pop is called, the key from the 
// (key, value) pair with the smallest value is returned; if 
// two pairs have the same value, the smaller key is returned.
// Push and pop have O(lgN) time complexity.
//
// Edited by: Jacob Janowski
//
// Original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #07
//

#pragma once

#include <iostream>
#include <vector>
#include <exception>
#include <map>
#include <stdexcept>

using namespace std;


template<typename TKey, typename TValue>
class minqueue
{
private:
    struct NODE
    {
      TKey    Key;
      TValue  Value;
    };
    NODE *arr;
    map<TKey, int>mapQueue;
    int heapSize;
    int arrSize;

    int getLeftChildIndex(int nodeIndex) {
      return 2 * nodeIndex + 1;
    }

    int getRightChildIndex(int nodeIndex) {
      return 2 * nodeIndex + 2;
    }

    int getParentIndex(int nodeIndex) {
      return (nodeIndex - 1)/2;
    }

public:

  // default constructor:
  //
  // Queue has a max capacity for efficient implementation.
  // This max capacity must be specified at queue creation.
  //
  minqueue()
  {
    //
    // TODO:
    arr = new NODE[26];
    heapSize = 0;
    
  }

  minqueue(int capacity)
  {
    //
    // TODO:
    arr = new NODE[capacity];
    heapSize = 0;
    arrSize = capacity;
    
  }
  
  //
  // fill constructor:
  //
  // This allows for the efficient O(N) construction of
  // a queue with an initial set of keys, all with the same
  // initial value.  The max capacity of the queue is 
  // set to the # of keys provided for initialization;
  // it is assumed the keys are in ascending order.
  //
  minqueue(vector<TKey> keys, TValue initialValue)
  {
    //
    // TODO:
    
    arrSize = keys.size();
    heapSize = keys.size();
    arr = new NODE[arrSize];
    int i = 0;
    

    for ( TKey v : keys)
    {
      arr[i].Key = v;
      arr[i].Value = initialValue;
      mapQueue[v] = i;
      i++;
    }
    


  }
  
  //This function is used to decerease the size of the array
  //and to erase the value to remove from the map.
  void eliminateIndex(TKey key, TValue value) {

    int pos = mapQueue[key];
    mapQueue.erase(key);
    if (pos - 1 == heapSize) {
      heapSize--;
      return;
    }
    int newHeap = heapSize - 1;
    arr[pos].Key = arr[newHeap].Key;
    arr[pos].Value = arr[newHeap].Value;

    heapSize--;
    mapQueue[arr[pos].Key] = pos;

    siftUp(pos);
    siftDown(pos);
    


  }
  //
  // destructor:
  //
  virtual ~minqueue()
  {
    //
    // TODO:
    delete[] arr;

  }
  

  //
  // empty:
  //
  // Returns true if empty, false if not.
  //
  bool empty()
  {
    if(heapSize == 0) {
      return true;
    } else {
      return false;
    }
  }


  //
  // push:
  //
  // Inserts the given (key, value) pair into the queue such that 
  // pop always returns the pair with the minimum value.  If the 
  // key is *already* in the queue, it's value is updated to the
  // given value and the queue reordered.  If the key is not in
  // the queue, the (key, value) pairs is added and the queue
  // reordered.
  //
  // NOTE: if two keys have the same value, i.e. (key1, value) and
  // (key2, value), then those pairs are ordered into ascending value
  // by their key.
  //
  void siftUp(int nodeIndex) {
  
    do 
    {
      int parent = getParentIndex(nodeIndex);
      
      //If the parent value is greater than the child value, we need to swap the values.
      if ((arr[parent].Value > arr[nodeIndex].Value) || 
          (arr[nodeIndex].Value == arr[parent].Value && 
          arr[parent].Key > arr[nodeIndex].Key))
      {
        
        //Create a temp node so that we can swap key, value pairs 
        //and store within the map and update.
        NODE temp = arr[nodeIndex];
        arr[nodeIndex].Key = arr[parent].Key;
        arr[nodeIndex].Value = arr[parent].Value;

        mapQueue[arr[nodeIndex].Key] = nodeIndex;
        arr[parent].Key = temp.Key;
        arr[parent].Value = temp.Value;

        mapQueue[arr[parent].Key] = parent;
        nodeIndex = parent;
      }
      else
      {
        break;
      }
      
      
    } while(nodeIndex > 0);

  }

  void pushinorder(TKey key, TValue value)
  {
    
    //
    // we need to insert a new (key, value) pair but the queue is full:
    //

    //eliminate when we encounter a key to delete.
    if (mapQueue.count(key) == 1)
    {
      eliminateIndex(key, value);
    }
    
    if (heapSize != arrSize)
    {
      arr[heapSize].Value = value;
      arr[heapSize].Key = key;
      mapQueue[key] = heapSize;

      siftUp(mapQueue[key]);
      heapSize++;
    }
    
    else { 
      
      throw runtime_error("minqueue::pushinorder: queue full");

    }
    
  }


  //
  // front:
  //
  // Returns the key at the front of the queue; does *not* pop the 
  // (key, value) pair.  Throws a logic_error exception if the queue
  // is empty.
  //
  TKey minfront()
  {
    if (empty())
    {
      throw logic_error("minqueue::minfront: queue empty");
    }
    else {
      return arr[0].Key;
    }

  }


  //
  // pop:
  //
  // Pops and discards the (key, value) pair at the front of the queue.
  // Throws a logic_error exception if the queue is empty.
  //
  void siftDown(int nodeIndex) {

	int leftChild = getLeftChildIndex(nodeIndex);
	int rightChild = getRightChildIndex(nodeIndex);
	int minIndex = nodeIndex;
  bool checkIfEqual = true;

    if(leftChild < heapSize){
      if(arr[leftChild].Value < arr[nodeIndex].Value ||
            (arr[leftChild].Value == arr[nodeIndex].Value && arr[leftChild].Key < arr[nodeIndex].Key)){
          minIndex = leftChild;
      }
    }
    if(rightChild < heapSize){
      if(arr[rightChild].Value < arr[minIndex].Value ||
            (arr[rightChild].Value == arr[minIndex].Value && arr[rightChild].Key < arr[minIndex].Key)){
          minIndex = rightChild;
      }
    }
    while(checkIfEqual) {
      if(minIndex != nodeIndex){
          swapArray(nodeIndex, minIndex);
        }
        checkIfEqual = false;
      }
    
  }

  //swap function needed in order to update map and then we need to sift down.
  void swapArray(int nodeIndex, int minIndex) {

      swap(arr[nodeIndex],arr[minIndex]);
      mapQueue[arr[nodeIndex].Key] = nodeIndex;
      mapQueue[arr[minIndex].Key] = minIndex;
      siftDown(minIndex);

  }



  void minpop()
  {
    if (!empty())
    {
      mapQueue.erase(arr[0].Key);
      int heapIndex = heapSize - 1;
      arr[0].Key = arr[heapIndex].Key;
      arr[0].Value = arr[heapIndex].Value;
      heapSize--;
      mapQueue[arr[0].Key] = 0;
      siftDown(0);
    }
    else
    {

      throw logic_error("minqueue::minpop: queue empty");

    }
    
   
  }

};