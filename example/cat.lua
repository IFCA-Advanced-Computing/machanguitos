function Agent:init()
   io.write( "** cat init\n" )
end

function Agent:update( delta )
   io.write( "** cat update = " .. delta .. "\n" )
end
