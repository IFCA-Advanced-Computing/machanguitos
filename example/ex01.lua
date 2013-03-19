io.write( "Cow Example (Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. ")\n" );

config.add_agent( "cow", 10 );
config.add_agent( "cat", 3 );

config.setvars( {
                   dbhost = "localhost",
                   dbport = 27017,
                   starttime = 0.0,
                   endtime = 10.0,
                   iters = 10,
         } );
