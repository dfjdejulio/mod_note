# mod_note

## Introduction

This is an Apache module for adding a "note" to Apache's internal data
structures.  Why bother?  Well, I use it for analytics.  A note can be
logged to Apache's log files by using the %{notename}n syntax.  So, if
you set things up right, you can trivially correlate which log entries
were "covered" by which .htaccess files just by reading the info
directly out of the logs.  (There are more complex uses too.)

## Example

If you were to create an .htaccess file with the following contents...
```
Note Author dfjdejulio
```
...and you were to put the following lines in your httpd.conf...
```
LogFormat "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-Agent}i\" \"%{Author}n\"" localcustomlog
CustomLog "/tmp/authorlog.txt" localcustomlog
```
...you would get a custom log in /tmp/authorlog.txt that showed a
"-" as its final field for most requests, but "dfjdejulio" as its
final field for requests in-scope for that .htaccess file.

You might take advantage of this by using that field to slice up
your log files before feeding them into analytics software, so
individual authors could get analytics for their own content but
not anyone else's.
