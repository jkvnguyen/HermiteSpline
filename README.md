<h1> Hermite Spline Class </h1>

[Video Demo](https://youtu.be/M9JSoGzx4tY)

Implementation with: 

* Maximum 40 control points
* Catmull-Rom initialization with second order accurate boundary conditions.
* Interactive spline creation (clicking adds a point).
* Manipulation of control points and tangents using the shell:
	* system {class name} set tangent {index} {x y z}
  * system {class name} set point {index} {x y z}
  * system {class name} add point {x y z sx sy sz} <br>      # adds a point with the given tangent at the end of the spline
* Arc Length parameterization using a piecewise linear approximation and look up table. The following function prints the arc length of a spline up to parametric value t. Parameter t in this case, goes from 0 to 1 for the entire spline, that is for t =1 you get the length of the entire spline.
	* system {class name} getArcLength {t}
* Input/output from/to a file:
	* system {class name} load {file name}
	* system {class name} export {file name}
	  * Format for the spline file: <br>
	  	{Spline Name} {n} <br>
		  {p1[x] p1[y] p1[z] s1[x] s1[y] s1[z]} <br>
	  	{p2[x] p2[y] p2[z] s2[x] s2[y] s2[z]} <br>
	  	…. <br>
	  	{pn[x] pn[y] pn[z] pn[x] sn[y] sn[z]} <br>
      <br>
  	where the prefix “p” refers to control points and the prefix “s” to tangents. When reading a file, ignore the name of the spline stored in it. The template code does not allow renaming of objects.
