const GRADE_TO_GPA = {
  "A": 4.0,
  "AB": 3.5,
  "B": 3.0,
  "BC": 2.5,
  "C": 2.0,
  "D": 1.0,
  "F": 0.0
}

function mapGPAs(students) {
  let newGPAs = [];
  students.forEach(student => {
    let newStudent = {};
    newStudent.name = student.name;
    newStudent.gpa = GRADE_TO_GPA[student.letterGrade];
    newGPAs.push(newStudent);
  });
  return newGPAs;
}

// Expected Output: []
mapGPAs([]);

// Expected Output: [ { name: 'Alice', gpa: 4.0 } ]
mapGPAs([{ name: 'Alice', letterGrade: 'A' }]) 

// Expected Output: [ { name: 'Alice', gpa: 4.0 }, { name: 'Bob', gpa: 3.0 }, { name: 'Charlie', gpa: 2.0 } ]
mapGPAs([ { name: 'Alice', letterGrade: 'A' }, { name: 'Bob', letterGrade: 'B' }, { name: 'Charlie', letterGrade: 'C' } ])