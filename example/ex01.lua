io.write( "Cow Example (Machen " .. config.VERSION_MAJOR .. "." .. config.VERSION_MINOR .. ")\n" );

config.setLogLevel( "debug" );

data.createLayer( "grass", 
                  {
                     x0 = 0.0,
                     x1 = 20.0,
                     y0 = 0.0,
                     y1 = 20.0,
                     w = 100,
                     h = 100,
                  } );

config.addAgent( "cow", 10 );
config.addAgent( "cat", 3 );

config.setvars( {
                   dbhost = "localhost",
                   dbport = 27017,
                   starttime = 0.0,
                   endtime = 10.0,
                   iters = 10,
         } );
