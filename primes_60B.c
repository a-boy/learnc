#include <stdio.h>
_=1;main($){for(;_%$?:(_+=$=1);++$*$>_&&printf("%i|",$=_));}

/*
作者也给出了注释：

_=1; // create integer _ and initialize with 1
main($){ // creates integer $ and initialize with
         // argument count (argc), in this case- 1
  for(
    ; _%$?:(_+=$=1)
         // Extended Code:
         //       if(_%$==0){ // if _ is divisible by $,
         //                   // then _ is not a prime number
         //         $=1; // reinitialize $ to 1
         //         _=_+$; // increase _ by 1
         //       }
    ; ++$*$>_&&printf("%i|",$=_)
         // Extended Code:
         //       ++$; // increase $ by one
         //       if($>sqrt(_)){ // if $ is larger than square
         //                      // root of _, then _ is a
         //                      // prime number.
         //         printf("%i|",_); // print _
         //         $=_; // initialize $ with _,
         //              // so that _%$ becomes 0 and next
         //              // prime number can be searched
         //       }
  );
}
*/