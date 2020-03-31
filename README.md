OpenVent 

Introduction 

Assumptions

Design Principles

Restrictions

It is our intention to rework the code over time to improve the safety margins and reliability by reworking the code against a industry recognised standard. As part of this we are/will be applying based off NASA's The Power of Ten (http://spinroot.com/gerard/pdf/P10.pdf). We have listed them below, for reference when making changes to the code. 
1. Restrict all code to very simple control flow contructs. This means NO goto, setjmp, longjmp or recursion. 
2. All loops must have fixed upper bounds. It must be easy to test if the upper bounds have been exceeded. 
3. After initialisation, there should be no dynamic memory allocation. 
4. No greater than 60 lines of code per function. Controls the amount of memory. 
5. On average no less than 2 assertions per function. Each function should have some form of check in 
6. Data objects must be declared at the smallest scope. Avoid declaring filtered or changed data objects. 
7. Check the validity of the parameters used inside the function it's being used in.
8. The use of preprocessor must be limited to the inclusion of header files and simple marcos. All macros will be expressed in complete syntax in the code. 
9. Restrict the use of pointers to one layer of referencing. 
10. Make sure you have you complier set to it's most stringent, make sure the code executes with ANY errors. 

Limitations

Improvement for next version
  - Pressure sensor + buzzer
  - Test pressure sensor with test lung
  - Impliment safety alarm (auidable & visual) code with pressure sensor (including timeouts)
  - Secure the bag down better
  - Remove global variables
  - Add LCD screen + button shield to read back pressure valves & other valves
  - Add auto switch-over to battery back up
  - Add voltage divider & safety code to give auidable & visual alarm when battery is running out
