---
layout: post
title:  "Create a Logo with Mathematica"
date:   2015-03-08 11:00
Categories: mathematica
---

I finally want to start the redoing of my website. One part is a new logo, that should combine the topics Computer Science, Mathematics and Music. I want to use the head of a double bass as the central image element. I try for a couple of years to walk the bass, but it is as always: too many things are more important and consume my spare time.

To keep mathematics in the equation, the headstock should be drawn with a Logarithmic Spiral. So first let's use Mathematica to draw one.

## Draw the Spiral

The general formula of the Logarithmic Spiral is the following:

![Formular of a Logarithmic Spiral](/images/mth_curl_formular.png)

The free parameters `a` and `b` must be chosen with care. `b` is relatively easy: if `t` is incremented by one an full rotation should be added. So we choose 2 π for `b`. For `a` I found the value 1/2 pleasing:

````mathematica
a = 1/2; b = 2 Pi;
curl[t_] := {Exp[a t] Cos[b t], Exp[a t] Sin[b t]}
````

To draw the spiral, we need to choose meaningful start and end values for `t`:

````mathematica
minCurl = -5; maxCurl = 5/16;
ParametricPlot[curl[t], {t, minCurl, maxCurl}]
````

![Graph of Spiral](/images/mth_curl_norm.png)

Because I need a mirrored image of the Spiral, I negate the y value:

````mathematica
curl[t_] := {Exp[a t] Cos[b t], -Exp[a t] Sin[b t]}
````

![Graph of inverted Spiral](/images/mth_curl_inv.png)

### Combine the parts

The bass headstock contains three parts: the Spiral and two short lines. These parts will be combined by BSplines.

To use an uniform value range, here is a function, that maps the range from 0 to 1 onto the range from `minCurl` to `maxCurl`:

````mathematica
curlRange[x_] := (maxCurl - minCurl) * x + minCurl
````

That results in a new plot command:

````mathematica
ParametricPlot[curl[curlRange[x]], {x, 0, 1}]
````

But the resulting image is still the same.

The two lines each have a start coordinate and a length. The x coordinate and the length are the same for both lines. Only the y coordinates differ.

````mathematica
startX = -3; width = 1;
upperY = 1/3; lowerY = -1/2;
````

Now we can plot the three parts in one diagram:

````mathematica
ParametricPlot[{
  curl[curlRange[x]],
  {x * width + startX, upperY},
  {x * width + startX, lowerY}
}, {x, 0, 1}]
````

![three parts](/images/mth_curl_parts.png)

### Tangents at the Spiral

To splice the BSplines seamlessly, we need the tangent vectors for points on the Spiral. To make things easy, I will use the vector that is perpendicular to the vector from the origin. This can be easily achieved by swapping the components:

````mathematica
tangent[t_, o_] :=  Module[
  {p = curl[t], q}, 
  q = {p[[2]], -p[[1]]}; p + o q
]
````

Let's name the point on the Spiral, where the upper line touches the Spiral `cutCurl`. Then we have:

````mathematica
cutCurl = -5/8;
ParametricPlot[{curl[curlRange[x]], tangent[cutCurl, x]}, {x, 0, 1}]
````

![tangent on the Spiral](/images/mth_curl_tangent.png)

That looks good.

### Finalize

Now we can add the BSplines between the parts:

````mathematica
ParametricPlot[{
    curl[curlRange[x]],
    {x * width + startX, upperY},
    {x * width + startX, lowerY}
  }, {x, 0, 1},
  Epilog -> {
    BSplineCurve[{
      {startX + width, upperY},
      {startX + width + 1/2, upperY},
      tangent[cutCurl, 3/2],
      curl[cutCurl]
    }],
    BSplineCurve[{
      {startX + width, lowerY},
      {startX + width + 1, lowerY},
      tangent[maxCurl, 1/2],
      curl[maxCurl]
    }]
  }
]
````

![added BSplines](/images/mth_curl_combined.png)

Now we must color all lines black, use a common line thickness and hide the coordinate system:

````mathematica
ParametricPlot[{
    curl[curlRange[x]],
    {x * width + startX, upperY},
    {x * width + startX, lowerY}
  }, {x, 0, 1},
  Axes -> False,
  PlotStyle -> {AbsoluteThickness[1]},
  ColorFunction -> (Black &),
  Epilog -> {
    BSplineCurve[{
      {startX + width, upperY},
      {startX + width + 1/2, upperY},
      tangent[cutCurl, 3/2],
      curl[cutCurl]
    }],
    BSplineCurve[{
      {startX + width, lowerY},
      {startX + width + 1, lowerY},
      tangent[maxCurl, 1/2],
      curl[maxCurl]
    }]
  }
]
````

![final Spiral](/images/mth_curl_final.png)

With Sketch I added some text and achieved the following:

![Logo](/images/mth_curl_icon.png)

It wasn't that easy to cut out the text. But that may be part of another entry.

----

#### 12/03/2015 Hal:

Can't you use Mathematica to calculate the correct tangent? Your approximation looks sluggish.

#### 12/03/2015 Timm:

@Hal: You are right. I plan another blog post with the correct solution.