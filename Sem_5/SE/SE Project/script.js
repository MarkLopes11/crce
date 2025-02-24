const daysList = document.getElementById('days-list');
const teachersList = document.getElementById('teachers-list');
const subjectsList = document.getElementById('subjects-list');
const scheduleTableBody = document.getElementById('schedule-tbody');
const generateScheduleBtn = document.getElementById('generate-schedule-btn');

generateScheduleBtn.addEventListener('click', generateSchedule);

function generateSchedule() {
  const days = Array.from(daysList.children);
  const teachers = Array.from(teachersList.children);
  const subjects = Array.from(subjectsList.children);

  const teacherSubjectMap = {
    'Prof.Prasad Lalit/Prof.Unik Lokhande': 'Honours',
    'Dr.B.S.Daga': 'Software Engineering',
    'Prof.Prajakta Dhamaskar': 'T.C.S',
    'Prof.Sunil Surve': 'PGM',
    'Prof.Merly Thomas': 'Computer Network',
  };

  const schedule = [];

  for (let i = 0; i < days.length; i++) {
    const day = days[i].textContent;
    const daySchedule = [];

    // 8:45 - 9:45
    const teacher1 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject1 = teacherSubjectMap[teacher1];
    daySchedule.push({ timeSlot: '8:45 - 9:45', teacher: teacher1, subject: subject1 });

    // 9:45 - 10:45
    const teacher2 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject2 = teacherSubjectMap[teacher2];
    daySchedule.push({ timeSlot: '9:45 - 10:45', teacher: teacher2, subject: subject2 });

    // 10:45 - 11:00 (short break)
    daySchedule.push({ timeSlot: '10:45 - 11:00', break: true });

    // 11:00 - 12:00
    const teacher3 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject3 = teacherSubjectMap[teacher3];
    daySchedule.push({ timeSlot: '11:00 - 12:00', teacher: teacher3, subject: subject3 });

    // 12:00 - 1:00
    const teacher4 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject4 = teacherSubjectMap[teacher4];
    daySchedule.push({ timeSlot: '12:00 - 1:00', teacher: teacher4, subject: subject4 });

    // 1:00 - 1:30 (break)
    daySchedule.push({ timeSlot: '1:00 - 1:30', break: true });

    // 1:30 - 2:30
    const teacher5 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject5 = teacherSubjectMap[teacher5];
    daySchedule.push({ timeSlot: '1:30 - 2:30', teacher: teacher5, subject: subject5 });

    // 2:30 - 3:30
    const teacher6 = teachers[Math.floor(Math.random() * teachers.length)].textContent;
    const subject6 = teacherSubjectMap[teacher6];
    daySchedule.push({ timeSlot: '2:30 - 3:30', teacher: teacher6, subject: subject6 });

    schedule.push({ day, daySchedule });
  }

  scheduleTableBody.innerHTML = '';

  schedule.forEach((scheduleItem) => {
    let counter = 1;
    console.log(counter)
    scheduleItem.daySchedule.forEach((scheduleItemRow) => {
      const row = document.createElement('tr');
      if (scheduleItemRow.break) {
        if (counter === 1){
          row.innerHTML = `
          <td rowspan="8">${scheduleItem.day}</td>
          `
          counter = 0
        }
        row.innerHTML += `
          <td>${scheduleItemRow.timeSlot}</td>
          <td colspan="2">Break</td>
        `;
      } else {
        if (counter === 1){
          row.innerHTML = `
          <td rowspan="8">${scheduleItem.day}</td>
          `
          counter = 0
        }
        row.innerHTML += `
          <td>${scheduleItemRow.timeSlot}</td>
          <td>${scheduleItemRow.teacher}</td>
          <td>${scheduleItemRow.subject}</td>
        `;
      }
      scheduleTableBody.appendChild(row);
    });
  });
}