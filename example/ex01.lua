io.write( "Cow Example (Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. ")\n" );

config.add_agent( "cow", 100 );

config.setvars( {
            starttime = 0.0,
            endtime = 10.0,
            iters = 100
         } );
