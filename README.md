***If you’re a student trying to look to start early, any info in this repo is not yet finalized***

Instructions For Modifying 33 Website

To modify the website, go to /course/cs0330/www/ and
open index.html. This is THE HTML for the website. You can comment
out any assignment/thing you don't want on the website, and it
instantly updates! We DO recommend commenting, not deleting, as
you'll have to put it back later.

Documents should be edited and modified in Google Drive, then
downloaded and placed in the appropriate subfolder in
/course/cs0330/www/docs. Links to these documents in the
HTML should point to docs/<folder>/<assignment>.pdf.

Staff pictures need to be in the img folder in .jpg format and must
be called <ta-login>.jpg.

The character photos need to be called <ta-login>-char.jpg, and must
be placed under the img/characters folder.

Each staff member's div must be updated manually to make the id
the staff member's login, and the footer text reflect the actual
name and login of the TA. Finding the pictures is handled automatically,
see js/cs33.js for more details!

Lecture slides should be updated automatically, but at this point
we have to manually add the lecture videos. We also record Gear-ups.

To create a web handout, the doc should be converted to markdown and
them to html using cs0330_convert_handout. You can then copy the result into  the
body section of a copy of webhandouts.html, and put it in the appropriate course folder. 
