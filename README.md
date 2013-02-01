CuTAES COMP2404 Project
=======================

By Zack Dawson 100851846
------------------------

***Description***
    The application is broken down into a series of *panels*, which represent a single screen. Panels contain an arbitrary number of *Components*. Components can represent *buttons*, text areas, forms, etc., but currently are limited to just buttons.
    Some panels are singletons; these panels are unique, so there is only every one instance created. Most menu panels fall under this category. Other panels are created as needed, and are usually some sort of form; such as the StudentInfoPanel, which gets various data from the user.
    The button system uses callbacks to call arbitrary event handlers when clicked. Additionally, components can register for any arbitrary event triggers with the panel they're contained in; currently, only buttons exist, so the *enter* key is the only trigger used. When more advanced components are created, this system will be used more extensively.
    Some global functions are used. These functions are either callbacks or utility functions; although the assignment description says to avoid global functions, they are necessary for this design. Callbacks could not be implemented without static functions, and utility functions greatly simplify the design without breaking object orientedness.
    Currently, courses are read from a file to populate the course list; however, they aren't placed in a scrollable panel. This means that having a lot of courses defined will result in the list going off-screen. Also, the selected course has no affect on the TA application; there is no application class yet. Currently, an application is just a bunch of text entered by the user. Application status has no meaning in the program yet, but it does save "pending" to the file.
    The student info entry screen will not allow the user to proceed by hitting enter until all fields are filled in. When the user continues from this screen, the information is saved to a file. Depending on future assignments, this may be changes so that student info is saved with the application; however, given the current requirements it makes sense to store the student information separately from each individual application.
