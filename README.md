OpenVent 

Introduction 

Ventilation is the only known available treatment for sufferers of COVID-19. Existing ventilation machines in hospitals are complex general purpose machines costing £10s of thousands. The availability of the existing ventilators is no where near enough to meet the predicted required numbers, for example in the UK around 5000 ventilators currently exist within the NHS but it is predicted they will need around 30,000 in just a few weeks. Critically ill patients left without ventilation treatment are in danger of losing their life.

As far as I understand, one effect of the virus as well as damaging the lungs, it also produces a very sticky mucus in the lungs which causes the lungs to collapse and makes it very difficult for the patent to breath of their own accord. Ventilation can be delivered either via intubation or a well sealed mask and delivers continuous positive air pressure into the lungs to keep them inflated at all times. Conscious less ill patents can be aided using a CPAP (Continuous Positive Airway Pressure) ventilation device like a sleep apnea device, this delivers a constant air of the same flow rate and pressure. The more critically ill patients need a machine to breath for them which varies the pressure and flow according to inhale and exhale and relies on maintaining a good seal and a PEEP valve to keep the lungs continually inflated.

An analogy: Imagine inflating a balloon, letting it deflate completely, then re-inflating it, that is like CPAP. If you inflated the balloon, deflated it half way, then re-inflated it, that's more like the treatment needed for the worse sufferers.
Assumptions

Design Principles

1. for a very simple low cost design.

2. made using readily available components.

3. that can be manufactured quickly and easily in small quantities or on mass at low cost.

4. to work reliably and with the lowest risk to the patient.

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

-At this time these rules have only be partly applied to the current version of the code, do not assume that these have been enforced.

-There is no test currently for checking if the motor has been wired correctly. So testing without a bag hooked up is nessesary.

-Doesn't meet the NHS requirements at this time, but it is the intention to get as close as practicable. 


Improvement for next version
  - Pressure sensor + buzzer
  - Test pressure sensor with test lung
  - Impliment safety alarm (auidable & visual) code with pressure sensor (including timeouts)
  - Secure the bag down better
  - Remove global variables
  - Add LCD screen + button shield to read back pressure valves & other valves
  - Add auto switch-over to battery back up
  - Add voltage divider & safety code to give auidable & visual alarm when battery is running out
  - Code in the motorSpeed POT to allow adjustment for different motors
