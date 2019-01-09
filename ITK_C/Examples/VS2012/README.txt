#
# README
#

Open the Examples.sln to build any of the examples (not the individual projs) since the solution has the library dependencies.

IRI_Empty has been provided as an empty template to easily try out adding custom IRI commands.

To add a new project to the solution:

CREATE PROJECT
* Open Examples.sln
* Right click solution -> Add -> New Project (Win32 Console Application, Empty Project, no Security Development Lifecycle)
* Make new project
* Remove projects existing filters (Headers, source, etc)

ADD FILES
* Right click proj -> Add -> Existing Item (then point to your example.c)

SET PROPERTY SHEET (contains include paths)
* Open property manager (View -> Other Windows -> Property Manager)
* Right click proj -> Add Existing Property Sheet -> point to to Examples.props

SET SOLUTION DEPENDENCIES (make sure the libraries are build before the example)
* On the solution, right click -> Project Dependencies
* Select new proj, then click the checkbox for ipj_util and IRI
