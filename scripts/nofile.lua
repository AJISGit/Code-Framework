local textStages = {'N', 'No', 'No ', 'No F', 'No Fi', 'No Fil', 'No File', 'No File\nP', 'No File\nPr', 'No File\nPro', 'No File\nProv', 'No File\nProvi', 'No File\nProvid', 'No File\nProvide', 'No File\nProvided'}
local stage = 1

Init(500, 500, 'No File')

local startIncrementTime = os.clock()

function onUpdate()
    
    if (stage < #textStages) and (os.clock() - startIncrementTime >= 0.3) then
        stage = stage + 1
        startIncrementTime = os.clock()
    end
    
end

function onDraw()
    -- SetDrawColor(0, 0, 0, 0)
    ClearWindow()
    SetDrawColor(255, 255, 255, 255)
    DrawText(textStages[stage], 125, 225, 70)
    
end