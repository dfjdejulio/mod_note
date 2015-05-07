# mod_note

This is an Apache module for adding a "note" to Apache's internal data
structures.  Why bother?  Well, I use it for analytics.  A note can be
logged to Apache's log files by using the %{notename}n syntax.  So, if
you set things up right, you can trivially correlate which log entries
were "covered" by which .htaccess files just by reading the info
directly out of the logs.  (There are more complex uses too.)
