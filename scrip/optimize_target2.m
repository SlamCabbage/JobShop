load("matlab.mat")
Untitled(319:end,:) = [];
drive_form = Untitled(:,4);
drive_form = table2array(drive_form);
head = drive_form(1);
score = 100;
pre = 1;
for i = 2 : length(drive_form)
    if drive_form(i-1) == 1-head && drive_form(i) == head
        i
        if mod(i - pre, 2) ~= 0
            score = score - 2;
        end
        for j = pre : i-1
            count = 0;
            if drive_form(j) == head
                count = count + 1;
            end
            if count ~= mod(i - pre, 2)
                score = score - 1;
            end
        end
    end
    pre = i;
end
score