const amqp = require('amqplib');

const RABBITMQ_URL = 'amqp://localhost';
const QUEUE_NAME = 'job_queue';

async function connectAndPublish() {
    try {
        const connection = await amqp.connect(RABBITMQ_URL);
        const channel = await connection.createChannel();

        await channel.assertQueue(QUEUE_NAME, { durable: true });

        const jobs = [
            "Job 1: Process User Data",
            "Job 2: Generate Report PDF",
            "Job 3: Send Email Notification",
            "Job 4: Backup Database"
        ];

        for (let i = 0; i < jobs.length; i++) {
            const msg = jobs[i];
            channel.sendToQueue(QUEUE_NAME, Buffer.from(msg), {
                persistent: true
            });
            console.log(`✅ [Producer] Sent job: '${msg}'`);
        }

        setTimeout(() => {
            connection.close();
            process.exit(0);
        }, 500);

    } catch (error) {
        console.error("❌ Error in Producer:", error.message);
        process.exit(1);
    }
}

connectAndPublish();
