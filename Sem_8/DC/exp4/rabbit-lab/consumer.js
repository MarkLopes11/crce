const amqp = require('amqplib');

const RABBITMQ_URL = 'amqp://localhost';
const QUEUE_NAME = 'job_queue';

async function connectAndConsume() {
    try {
        const connection = await amqp.connect(RABBITMQ_URL);
        const channel = await connection.createChannel();

        await channel.assertQueue(QUEUE_NAME, { durable: true });

        channel.prefetch(1);

        console.log("⏳ [Consumer] Waiting for jobs in %s. To exit press CTRL+C", QUEUE_NAME);

        channel.consume(QUEUE_NAME, (msg) => {

            if (msg !== null) {
                const jobContent = msg.content.toString();
                console.log(`➡️ [Consumer] Received job: '${jobContent}'`);

                const processingTime = Math.floor(Math.random() * 3000) + 1000;

                console.log(`[Consumer] Processing job... (simulated time: ${processingTime}ms)`);

                setTimeout(() => {
                    channel.ack(msg);
                    console.log(`✅ [Consumer] Finished and acknowledged: '${jobContent}'`);
                }, processingTime);
            }

        }, { noAck: false });

    } catch (error) {
        console.error("❌ Error in Consumer:", error.message);
        process.exit(1);
    }
}

connectAndConsume();
