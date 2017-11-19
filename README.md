
- Hermite spline class with a maximum of 40 control points.
- Catmull-Rom initialization with second order accurate boundary conditions.
- Interactive spline creation (clicking adds a point).
- Manipulation of control points and tangents using the shell:
	> system <name> set tangent <index> <x y z>
	> system <name> set point <index> <x y z>
	> system <name> add point <x y z sx sy sz> # adds a point with the given tangent at the end of the spline.
- Arc Length parameterization using a piecewise linear approximation and look up table. The following
  function call should print the arc length of a spline up to parametric value t. Parameter t in this case, goes from 0 to
  1 for the entire spline, that is for t =1 you get the length of the entire spline.
	> system <name> getArcLength <t>
- Input/output from/to a file.
		> system <name> load <file name>
		> system <name> export <file name>
	-Implement exactly the following format for the spline file:
		<Spline Name> <n>
		<p1[x] p1[y] p1[z] s1[x] s1[y] s1[z]>
		<p2[x] p2[y] p2[z] s2[x] s2[y] s2[z]>
		….
		<pn[x] pn[y] pn[z] pn[x] sn[y] sn[z]>
	where the prefix “p” refers to control points and the prefix “s” to tangents.
	When reading a file you have to ignore the name of the spline stored in it. The template code does
	not allow renaming of objects.