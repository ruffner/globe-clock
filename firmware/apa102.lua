apa102 = {}
    
function apa102.init()
    spi.setup(1, spi.MASTER, spi.CPOL_LOW, spi.CPHA_LOW, spi.DATABITS_8, 0);
end

function apa102.write(brightness, data)
    local led_count = string.len(data) / 3
    local output_buffer = ""
    local bright_char = string.char(brightness)

    for i=0,(led_count - 1) do
        output_buffer = output_buffer..
            bright_char..
            string.sub(
                data,
                (i * 3) + 1, 
                (i * 3) + 3
            )
    end

    return spi.send(
        1,
        string.rep(
            string.char(0x00),
            4
        )..output_buffer..string.rep(
            string.char(0xff),
            led_count * 2
        )
    )
end