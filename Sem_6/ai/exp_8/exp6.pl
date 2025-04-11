% employee(name,department number ,scale)
% department(department number,name department)
% salary(scale,amount)

employee(mcardon,1,5).
employee(treeman,2,3).
employee(chapman,1,2).
employee(claessen,4,1).
employee(petersen,5,8).
employee(cohn,1,7).
employee(duffy,1,9).

department(1,board).
department(2,human_resources).
department(3,production).
department(4,technical_services).
department(5,administration).

salary(1,1000).
salary(2,1500).
salary(3,2000).
salary(4,2500).
salary(5,3000).
salary(6,3500).
salary(7,4000).
salary(8,4500).
salary(9,5000).

selection(X,Y) :-
    call(X),
    call(Y),
    write(X),
    nl,
    fail.
selection(_,_).

projection(X,Y) :-
    call(X),
    write(Y),
    nl,
    fail.
projection(_,_).

join(X,Y,Z) :-
    call(X),
    call(Y),
    call(Z),
    write(X),
    write(Y),
    nl,
    fail.
join(_,_,_).