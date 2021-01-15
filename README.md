# [OpenVent-Bristol](https://openventbristol.co.uk/)

# Introduction

Ventilation is the only known available treatment for sufferers of COVID-19. One of the effects of the virus as well as damaging the lungs, is also that it produces a very sticky mucus in the lungs which causes the lungs to collapse and makes it very difficult for the patent to breath of their own accord. Ventilation can be delivered either via intubation or a well sealed mask and delivers continuous positive air pressure into the lungs to keep them inflated at all times. Conscious less ill patents can be aided using a CPAP (Continuous Positive Airway Pressure) ventilation device like a sleep apnea device, this delivers a constant air of the same flow rate and pressure. The more critically ill patients need a machine to breath for them which varies the pressure and flow according to inhale and exhale and relies on maintaining a good seal and a PEEP valve to keep the lungs continually inflated.

An analogy: Imagine inflating a balloon, letting it deflate completely, then re-inflating it, that is like CPAP. If you inflated the balloon, deflated it half way, then re-inflated it, that's more like the treatment needed for the worse sufferers.

# How to build OpenVent

## V2.0:
  - Instructables: https://www.instructables.com/id/OpenVent-Bristol-V20-COVID-19-Rapid-Manufacture-Ve/
  - CAD: https://grabcad.com/library/openvent-bristol-v2-0-covid-19-rapid-manufacture-ventilator-bvm-ambubag-2
  - Videos:
    - https://www.youtube.com/watch?v=RIKmd1mEYg4
    - https://www.youtube.com/watch?v=M4Qg_H6V9DY&t=2s
    - https://www.youtube.com/watch?v=Nv8YVDp8wuQ
    - https://www.youtube.com/watch?v=RtxiVJMbwBY

## V3.0:
  - CAD: https://grabcad.com/library/openvent-bristol-v3-0-covid-19-rapid-manufacture-ventilator-bvm-ambubag-1
  - Intro: https://www.youtube.com/watch?v=QZ1g1FmOCdQ
  - More to follow...

# Design information
## Assumptions

1. That whomever produces this is able to produce the physical components to a standard.
2. That on average a human breaths between 8-30 breathes per minute depending on a number of factors.  
3. Humans can start having negative effects from asphyxia in around 90 seconds.
4. That the figure of the global average of around 1 doctor to 10 intensive patients.
5. Power at some point could be lost or be inconsistent from source.

## Design Principles

1. for a very simple low cost design.
2. made using readily available components.
3. that can be manufactured quickly and easily in small quantities or on mass at low cost.
4. to work reliably and with the lowest risk to the patient.

## Restrictions

It is our intention to rework the code over time to improve the safety margins and reliability by reworking the code against a industry recognized standard. As part of this we are/will be applying based off NASA's The Power of Ten (http://spinroot.com/gerard/pdf/P10.pdf). We have listed them below, for reference when making changes to the code.

1. Restrict all code to very simple control flow constructs. This means NO goto, setjmp, longjmp or recursion.
2. All loops must have fixed upper bounds. It must be easy to test if the upper bounds have been exceeded.
3. After initialization, there should be no dynamic memory allocation. 
4. No greater than 60 lines of code per function. Controls the amount of memory.
5. On average no less than 2 assertions per function. Each function should have some form of check in
6. Data objects must be declared at the smallest scope. Avoid declaring filtered or changed data objects.
7. Check the validity of the parameters used inside the function it's being used in.
8. The use of preprocessor must be limited to the inclusion of header files and simple marcos. All macros will be expressed in complete syntax in the code.
9. Restrict the use of pointers to one layer of referencing.
10. Make sure you have you complier set to it's most stringent, make sure the code executes with ANY errors.

## Limitations

1. At this time these rules have only be partly applied to the current version of the code, do not assume that these have been enforced.
2. There is no test currently for checking if the motor has been wired correctly. So testing without a bag hooked up is necessary.
3. Doesn't meet the NHS requirements at this time, but it is the intention to get as close as practicable.

## Improvement for next version
  - Pressure sensor + buzzer
  - Test pressure sensor with test lung
  - Implement safety alarm (audible & visual) code with pressure sensor (including timeouts)
  - Secure the bag down better
  - Remove global variables
  - Add LCD screen + button shield to read back pressure valves & other valves
  - Add auto switch-over to battery back up
  - Add voltage divider & safety code to give audible & visual alarm when battery is running out
  - Code in the motorSpeed POT to allow adjustment for different motors

https://openventbristol.co.uk/
