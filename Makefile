MOD=mod_note

$(MOD).o: $(MOD).c
	apxs -c $(MOD).c

clean:
	-rm *.*o *~ *.la
