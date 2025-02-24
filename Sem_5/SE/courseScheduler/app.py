from flask import Flask, request, render_template, redirect, url_for, flash
import os
import re
from werkzeug.utils import secure_filename

app = Flask(__name__)
app.secret_key = 'supersecretkey'  # For flash messages
app.config['UPLOAD_FOLDER'] = 'uploads'
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024  # Limit file size to 16 MB

# Define data structures
class Room:
    def __init__(self, number, capacity):
        self.number = number
        self.capacity = capacity

class Course:
    def __init__(self, number, enrollment, preferences):
        self.number = number
        self.enrollment = enrollment
        self.preferences = preferences

class Schedule:
    def __init__(self):
        self.schedule = {}

    def add(self, time, room, course):
        if time not in self.schedule:
            self.schedule[time] = {}
        self.schedule[time][room] = course

    def get_schedule(self):
        return self.schedule

class UnscheduledCourses:
    def __init__(self):
        self.courses = {}

    def add(self, course, reasons):
        self.courses[course] = reasons

    def get_unscheduled_courses(self):
        return self.courses

class ErrorMessages:
    def __init__(self):
        self.errors = []

    def add(self, error):
        self.errors.append(error)

    def get_errors(self):
        return self.errors

# Utility functions
def parse_input_file_1(file_path):
    rooms = []
    courses = set()
    times = set()
    errors = ErrorMessages()

    # Regex pattern to validate time formats
    time_format_pattern = re.compile(r'^(MWF(\d{1,2})(:\d{2})?|TT(\d{1,2})(:\d{2})?)$')


    with open(file_path, 'r') as file:
        content = file.read().split('\n')

    try:
        section = None
        for line in content:
            line = line.strip()
            if line.startswith('rooms'):
                section = 'rooms'
                continue
            elif line.startswith('courses'):
                section = 'courses'
                continue
            elif line.startswith('times'):
                section = 'times'
                continue
            elif line == ';':
                section = None
                continue

            if section == 'rooms':
                match = re.match(r'(\d{3}) : (\d+)', line)
                if match:
                    room_number, capacity = match.groups()
                    capacity = int(capacity)
                    if 10 <= capacity <= 300:
                        rooms.append(Room(room_number, capacity))
                    else:
                        errors.add(f'Room {room_number} has invalid capacity {capacity}.')
                else:
                    errors.add(f'Invalid room format: {line}')
            elif section == 'courses':
                courses.update(line.split(','))
            elif section == 'times':
                time_list = line.split(',')
                for time in time_list:
                    time = time.strip()
                    if not time_format_pattern.match(time):
                        errors.add(f'Invalid time format: {time}')
                    else:
                        times.add(time)

        # Validate courses
        courses = {c.strip() for c in courses if re.match(r'cs\d{3}', c)}
        if len(courses) > 30:
            errors.add('More than 30 courses in the file. Later ones are ignored.')

    except Exception as e:
        errors.add(f'Error parsing file: {str(e)}')

    return rooms, courses, times, errors



def parse_input_file_2(file_path, valid_courses, valid_times):
    offered_courses = []
    unscheduled_courses = UnscheduledCourses()
    errors = ErrorMessages()

    with open(file_path, 'r') as file:
        content = file.read().split('\n')

    try:
        for line in content:
            line = line.strip()
            if not line:
                continue

            parts = line.split()
            course_number = parts[0]
            enrollment = int(parts[1])
            preferences = parts[2:] if len(parts) > 2 else []

            if course_number not in valid_courses:
                errors.add(f'Course {course_number} is not in the catalog.')
                continue

            if any(pref not in valid_times for pref in preferences):
                errors.add(f'Course {course_number} has invalid lecture times.')
                continue

            offered_courses.append(Course(course_number, enrollment, preferences))

    except Exception as e:
        errors.add(f'Error parsing file: {str(e)}')

    return offered_courses, unscheduled_courses, errors

def schedule_courses(rooms, offered_courses, valid_times):
    schedule = Schedule()
    unscheduled_courses = UnscheduledCourses()
    errors = ErrorMessages()

    try:
        for course in offered_courses:
            scheduled = False
            for pref in course.preferences:
                if not scheduled:
                    for room in rooms:
                        if course.enrollment <= room.capacity:
                            schedule.add(pref, room.number, course.number)
                            scheduled = True
                            break
                if scheduled:
                    break
            if not scheduled:
                unscheduled_courses.add(course.number, ['No room with proper capacity.'])

    except Exception as e:
        errors.add(f'Error scheduling courses: {str(e)}')

    return schedule, unscheduled_courses

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        if 'file1' not in request.files or 'file2' not in request.files:
            flash('Both files are required.', 'error')
            return redirect(url_for('index'))

        file1 = request.files['file1']
        file2 = request.files['file2']

        if not (file1 and allowed_file(file1.filename)) or not (file2 and allowed_file(file2.filename)):
            flash('Invalid file type. Only .txt files are allowed.', 'error')
            return redirect(url_for('index'))

        filename1 = secure_filename(file1.filename)
        filename2 = secure_filename(file2.filename)

        file1_path = os.path.join(app.config['UPLOAD_FOLDER'], filename1)
        file2_path = os.path.join(app.config['UPLOAD_FOLDER'], filename2)

        file1.save(file1_path)
        file2.save(file2_path)

        try:
            rooms, courses, times, errors1 = parse_input_file_1(file1_path)
            if errors1.get_errors():
                flash('\n'.join(errors1.get_errors()), 'error')
                return redirect(url_for('index'))

            offered_courses, unscheduled_courses, errors2 = parse_input_file_2(file2_path, courses, times)
            if errors2.get_errors():
                flash('\n'.join(errors2.get_errors()), 'error')
                return redirect(url_for('index'))

            schedule, unscheduled_courses = schedule_courses(rooms, offered_courses, times)

            return render_template('results.html', schedule=schedule.get_schedule(),
                                  unscheduled_courses=unscheduled_courses.get_unscheduled_courses(),
                                  errors=errors1.get_errors() + errors2.get_errors())
        except Exception as e:
            flash(str(e), 'error')
            return redirect(url_for('index'))
        finally:
            os.remove(file1_path)
            os.remove(file2_path)

    return render_template('index.html')

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in {'txt'}

if __name__ == '__main__':
    if not os.path.exists(app.config['UPLOAD_FOLDER']):
        os.makedirs(app.config['UPLOAD_FOLDER'])
    app.run(debug=True)
