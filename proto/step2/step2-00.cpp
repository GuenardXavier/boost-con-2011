/*******************************************************************************
 *           Getting Jiggy with Boost.Proto - Boost'Con 2011 Tutorial
 *                                 by Joel Falcou
 *
 *                      Step 1 : Putting Expressions to Work
 *
 *             Distributed under the Boost Software License, Version 1.0.
 *                    See accompanying file LICENSE.txt or copy at
 *                       http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#include <iostream>
#include "functions.hpp"
#include "terminals.hpp"

/*******************************************************************************
 * our EDSL has now been refurbished and split into convenient files.
 ******************************************************************************/

/*******************************************************************************
 * Now test our analytical function computation systems
 ******************************************************************************/

int main()
{
  std::cout << (cos(_x*3.14159))(0.25) << "\n";
  std::cout << (cos(_x)*cos(_x) + sin(_x)*sin(_x))(1.23456) << "\n";

}
