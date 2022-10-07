load("matlab.mat")
Untitled(319:end,:) = [];
engine_types = Untitled(:,3);
engine_types = table2array(engine_types);
first_one = true;
fisrt_one_ = 0;
score = 100;
for i = 1 : length(engine_types)
    if engine_types(i) && first_one
        first_one = false;
        fisrt_one_ = i;
    elseif engine_types(i) && ~first_one
        if(i - fisrt_one_ - 1 ~= 2)
            score = score - 1;
        end
        fisrt_one_ = i;
    end
end
score