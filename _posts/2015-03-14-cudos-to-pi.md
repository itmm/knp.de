---
layout: post
title:  "Cudos to π"
date:   2015-03-14 17:00
Categories: mathematics
---

This post is not directly related to Mathematica. Instead it is about the number π. This is because this week contains the best approximation to π you can get in this millennium (at least if you are using american notation): 3/14/15.

As you have surely learned in school, π is roughly

````mathematica
3.141592653589793238462643383279502884197169399375105820974944592307816406
````

Here I want to talk about a nice relation to π with the complex numbers. And how you can extract a bunch of trigonometry stuff from it, if you now complex exponentiation.

### Very short definition of the complex numbers

A complex number `z` can be treated as an ordered pair of two real numbers:

````mathematica
z = [z1, z2]
````

Two such numbers `z = [z1,z2]` and `y = [y1,y2]` can be added by adding there components:

````mathematica
z + y = [z1 + y1, z2 + y2]
````

and multiplied by this strange formula:

````mathematica
z * y = [z1 * y1 - z2 * y2, z1 * y2 + z2 * y1]
````

Basically we treat the second component as a factor for the strange number `i` whose square is defined to be -1.

### Complex Exponentiation

So what is `e` to the power `z`, if `z` is a complex number? Using the Taylor expansion, the e function can be written as

````mathematica
Exp[x_] := 1 + Sum[1/j! * x^j, j]
````

In this expansion we are only using addition and multiplication. Both are defined for complex numbers, so we use the Taylor expansion as a definition for the complex exponentiation.

### Coming to π

If use the Taylor expansion of the `Cos` and `Sin` functions, we can verify that

````mathematica
Exp[[x1, x2]] = Exp[x1] * [Cos[x2], Sin[x2]]
````

And therefore

````mathematica
Exp[[0, Pi / 2]] = [0, 1]
````

So we can define π as the double of the second entry of the inverse function:

````mathematica
Log[[0, 1]][[2]] * 2 = Pi
````

This is nice, because we found π solely by using the complex exponentiation and its inverse: the logarithm. No trigonometry needed (only to find the right value).

So good luck π, for the next millennia.

You can read more about π in Petr Beckmanns [A History of Pi].

[A History of Pi]: http://www.amazon.de/A-History-Pi-Peter-Beckman/dp/0312381859/ref=tmm_pap_title_0&tag=knpde-21
