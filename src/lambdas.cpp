#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>


using std::cout;
using std::endl;

void printHeader() {
   cout << endl;
   cout << "===============================================" << endl;
   cout << "Experiments with lambdas" << endl;
   cout << "===============================================" << endl;
   cout << endl;
}

struct Book {
   Book(const std::string & title) : my_title(title) {}
   std::string my_title;
   const std::string & title() const { return my_title; }
};

bool containsTitle_1( const std::vector<Book> & shelf, std::string wantedTitle ) {
   auto hasTitle = [t=wantedTitle](const Book& b) {
      return b.title() == t;
   };

   return shelf.end() != std::find_if(shelf.begin(), shelf.end(), hasTitle);
}

bool containsTitle_2( const std::vector<Book> & shelf, std::string wantedTitle ) {
   auto hasTitle = [pt = & wantedTitle](const Book& b) {
      return b.title() == *pt ;
   };

   return shelf.end() != std::find_if(shelf.begin(), shelf.end(), hasTitle);
}

bool containsTitle_3( const std::vector<Book> & shelf, std::string wantedTitle ) {
   auto hasTitle = [&t = wantedTitle](const Book& b) {
      return b.title() == t ;
   };

   return shelf.end() != std::find_if(shelf.begin(), shelf.end(), hasTitle);
}

void testContainsTitleLambdas() {
   cout << ">>>>> Testing containsTitle lambdas:" << endl;
   std::vector<Book> books = {{"Where Eagles Dare"}, {"Ice Station Zebra"}, {"The Guns of Navarrone"},
                              {"Fear Is the Key"}};
   cout << "Looking for the: " << "Ice Station Zebra" << ", result: "
        << containsTitle_1(books, "Ice Station Zebra") << endl;
   cout << "Looking for the: " << "The Witch" << ", result: " << containsTitle_2(books, "The Witch") << endl;
   cout << "Looking for the: " << "Where Eagles Dare" << ", result: "
        << containsTitle_2(books, "Where Eagles Dare") << endl;
   cout << "Looking for the: " << "When Eight Bells Toll" << ", result: "
        << containsTitle_2(books, "When Eight Bells Toll") << endl;
   cout << "Looking for the: " << "The Guns of Navarrone" << ", result: "
        << containsTitle_3(books, "The Guns of Navarrone") << endl;
   cout << "Looking for the: " << "HMS Ulysses" << ", result: " << containsTitle_3(books, "HMS Ulysses") << endl;
   cout << endl;
}

void testLambdaMutableState() {
   cout << ">>>>> Testing lambda mutable state:" << endl;
   auto counter = [i=0]() mutable { return ++ i; };
   auto c1 = counter;
   auto c2 = counter;
   cout << c1() << c1() << c1() << c1() << endl;
   cout << c2() << c2() << c2() << endl;
}

void testLambdasForEraseInWholeVector() {
   cout << ">>>>> Testing lambda for vector erase:" << endl;
   int arg[10] = {1, 2, 3, 3, 3, 5, 3, 4, 3, 10};
   std::vector<int> vec(arg, arg+10);

   cout << "Before erasing vec.size() = " << vec.size() << ":" << endl;
   for( int element : vec ) {
      cout << element << ", ";
   }
   cout  << endl;

   auto isToErase = [](int i){ return i == 3;};
   vec.erase( std::remove_if( vec.begin(), vec.end(), isToErase),
              vec.end());
   cout << "After erasing vec.size() = " << vec.size() << ":" << endl;
   for( int element : vec ) {
      cout << element << ", ";
   }
   cout  << endl;
}

void testLambdasForEraseSingleElement() {
   cout << ">>>>> Testing lambda for erasing single element:" << endl;
   int arg[10] = {1, 2, 3, 3, 3, 5, 3, 4, 3, 10};
   std::vector<int> vec(arg, arg+10);

   cout << "Before erasing vec.size() = " << vec.size() << ":" << endl;
   for( int element : vec ) {
      cout << element << ", ";
   }
   cout  << endl;

   auto isToErase = [](int i){ return i == 3;};
   auto pos = std::find_if(vec.begin(), vec.end(),isToErase);
   if ( pos != vec.end() ) {
      vec.erase( pos );
      cout << "Element erased" << endl;
   }
   else {
      cout << "Element not found" << endl;
   }
   cout << "After erasing vec.size() = " << vec.size() << ":" << endl;
   for( int element : vec ) {
      cout << element << ", ";
   }
   cout  << endl;
}

int g = 10;
auto cat = [g=g]() { return g+1; };

void printVector( const std::vector<std::string> & vectorToPrint ) {
   cout << endl;
   for ( std::string element : vectorToPrint) {
      cout << element << " ";
   }
   cout << endl;
}

void testNicolaiJosuttis_Example1() {
   std::vector<std::string> coll{"Here", "are", "some", "cities", "Berlin", "LA", "London", "Cologne"};
   printVector( coll );
   std::ranges::sort( coll );
   printVector( coll );
   std::ranges::sort( coll,
              [] (const std::string & s1, const std::string & s2) {
                 auto toUpper = [](char c){return std::toupper( c ); };
                 return std::ranges::lexicographical_compare( s1,
                                                              s2,
                                                              std::less{},
                                                              toUpper,
                                                              toUpper);
              }
   );
   printVector(coll);
}

void testNicolaiJosuttis_Example2() {
   std::vector<int> coll{0, 8, 15, 42, 11, 1, 77, -1, 3};
   int max = 30;
   int num = std::count_if( coll.begin(), coll.end(),
                            [max] (int elem) {
                               return elem < max;
                            });
   std::cout << "elems < " << max << " : " << num << endl;

   max = 10;
   num = std::count_if( coll.begin(), coll.end(),
                            [max] (int elem) {
                               return elem < max;
                            });
   std::cout << "elems < " << max << " : " << num << endl;

}

void testNicolaiJosuttis_Example3() {
   std::deque<int> coll{1, 3, 19, 5, 13, 7, 11, 2, 17};
   int min = 5;
   int max = 12;
   auto pos = std::find_if(coll.begin(), coll.end(),
                           [min, max](int i) {
                              return min <= i && i <= max;
                           });
   if ( pos != coll.end() ) {
      std::cout << "first found element: " << * pos << endl;
   }
}

void testNicolaiJosuttis_Example4() {
   std::cout << endl;
   std::cout << "Lambdas are stateless by deafult (not allowed to modified values captured by-value)" << endl;
   std::cout << "mutable makes them stateful (modifications allowed)" << endl;

   auto changed = [prev = 0] (auto val) mutable {
                     bool changed = (prev != val);
                     prev = val;
                     return changed;
                  };
   std::vector<int> coll {7, 42, 42, 0, 3, 3, 7};
   std::copy(coll.begin(), coll.end(),
             std::ostream_iterator<int>{std::cout, " "});
   cout << endl;
   std::copy_if(coll.begin(), coll.end(),
                std::ostream_iterator<int>{std::cout, " "},
                changed);
   cout << endl;
}

void testLambdaByReference1() {
   int val = 1;
   auto multiplyByVal = [& v = val](int input) {
                           return v * input;
                        };
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;

   val = 4;
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;
};

void testLambdaByReference2() {
   int val = 1;
   auto multiplyByVal = [&](int input) {
      return val * input;
   };
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;

   val = 4;
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;
};

void testLambdaByValue1() {
   int val = 1;
   auto multiplyByVal = [v = val](int input) {
      return v * input;
   };
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;

   val = 4;
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;
};

void testLambdaByValue2() {
   int val = 1;
   auto multiplyByVal = [val](int input) {
      return val * input;
   };
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;

   val = 4;
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;
};

void testLambdaByValue3() {
   int val = 1;
   auto multiplyByVal = [=](int input) {
      return val * input;
   };
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;

   val = 4;
   cout << "val = " << val << ", multiplyByVal(2) returns: " << multiplyByVal(2) << endl;
};

int main( int argc, char *argv[] ) {
   printHeader();

   std::atexit([]() {
      std::cout << endl << "good bye..." << endl;
   });

   auto plus = [value = 1](int x){ return x + value; };
   auto y = plus(43);
   cout << "y = " << y << endl;

   g = 14;
   auto kitten = [=]() { return g+1; };
   cout << "kitten: " << kitten() << ", cat: " << cat() << endl;
   g = 20;
   cout << "kitten: " << kitten() << ", cat: " << cat() << endl;
   //testContainsTitleLambdas();
   //testLambdaMutableState();
   //testLambdasForEraseInWholeVector();
   //testLambdasForEraseSingleElement();
   //testNicolaiJosuttis_Example1();
   //testNicolaiJosuttis_Example2();
   //testNicolaiJosuttis_Example3();
   //testNicolaiJosuttis_Example4();
   testLambdaByReference1();
   testLambdaByReference2();
   testLambdaByValue1();
   testLambdaByValue2();
   testLambdaByValue3();
   cout << endl;

   return 0;
}
