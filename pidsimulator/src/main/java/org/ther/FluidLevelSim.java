/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.ther;

import com.google.common.base.MoreObjects;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.block.BlockBorder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.*;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

/**
 * This one is meant only as exercise, it regulates fluid level in a cylindrical tank.
 * Water is flowing into a tank and it' coming out. As a result water level is changing.
 */
public class FluidLevelSim {
    private final Output out = new Output();
    private final Driver driver;


    /**
     * Diameter of the tank.
     */
    private static final float DIAMETER = 3;

    private static final int PROBES = 100;

    public static void main(String... args) {

        FluidLevelSim ex = new FluidLevelSim(new PIDDriver());
        ex.calculate();
    }


    public void calculate() {
        Output setPoint = new Output();
        setPoint.level = 2.5F;

        Input in = new Input();
        in.flowIn = 1;

        List<ProbePoint> probes = new ArrayList<>(PROBES);
        for (int sample = 0; sample < PROBES; sample++) {
            Output out = probe(in);
            PIDVal pidVal = driver.drive(setPoint, out, in);

            System.out.println(in + " " + out+ " -> "+pidVal);
            probes.add(new ProbePoint(in, out, setPoint, sample, pidVal));
            if(sample == 30){
                in.flowIn = 4f;
            }
        }

        ChartUI chartUI = new ChartUI();
        chartUI.createUI(probes);
        chartUI.setVisible(true);
    }

    public FluidLevelSim(Driver driver) {
        this.driver = driver;
    }

    public Output probe(Input in) {
        out.level += (in.flowIn + in.flowOut) / DIAMETER;
        if (out.level < 0) {
            out.level = 0;
        }
        return out;
    }


    interface Driver {
        PIDVal drive(Output setPoint, Output current, Input in);
    }

    static class PIDDriver implements Driver {
        private static final float AMP_P = 6;
        private static final float AMP_I = 2.5f;
        private static final float AMP_D = 1.4f;
        private float iDerivationSum = 0;
        private float dPrevDerivation = 0;

        @Override
        public PIDVal drive(Output setPoint, Output current, Input in) {
            float valP = calculateP(setPoint, current);
            float valI = calculateI(setPoint, current);
            float valD = calculateD(setPoint, current);

            float pid = valP + valI + valD;
            if (pid > 0) {
                pid = 0;
            }else  if (pid < -5 ){
                pid = -5;
            }
            in.flowOut = pid;
            return new PIDVal(valP, valI, valD, pid);
        }

        private float calculateP(Output setPoint, Output current) {
            float derivation = setPoint.level - current.level;
            return AMP_P * derivation;
        }

        private float calculateI(Output setPoint, Output current) {
            float derivation = setPoint.level - current.level;
            iDerivationSum += derivation;
            return AMP_I * iDerivationSum;
        }

        private float calculateD(Output setPoint, Output current) {
            float derivation = setPoint.level - current.level;
            float d = AMP_D * (derivation - dPrevDerivation);
            dPrevDerivation = derivation;
            return d;
        }
    }

    static class PIDVal implements Cloneable {
        float valP;
        float valI;
        float valD;
        float pid;

        public PIDVal(float valP, float valI, float valD, float pid) {
            this.valP = valP;
            this.valI = valI;
            this.valD = valD;
            this.pid = pid;
        }

        @Override
        protected PIDVal clone() {
            return new PIDVal(valP, valI, valD, pid);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("PID")
                    .add("valP", valP)
                    .add("valI", valI)
                    .add("valD", valD)
                    .add("pid", pid)
                    .toString();
        }
    }

    static class ProbePoint {
        Output out;
        Input in;
        Output setPoint;
        int sample;
        PIDVal pidVal;

        public ProbePoint(Input in, Output out, Output setPoint, int sample, PIDVal pidVal) {
            this.in = in.clone();
            this.out = out.clone();
            this.sample = sample;
            this.setPoint = setPoint.clone();
            this.pidVal = pidVal.clone();
        }
    }

    static class Input implements Cloneable {
        /**
         * water flowing into the tank.
         */
        float flowIn = 0;

        /**
         * water leaving the tank.
         */
        float flowOut = 0;


        public Input(float flowIn, float flowOut) {
            this.flowIn = flowIn;
            this.flowOut = flowOut;
        }

        public Input() {
        }

        @Override
        protected Input clone() {
            return new Input(flowIn, flowOut);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("IN")
                    .add("flowIn", flowIn)
                    .add("flowOut", flowOut)
                    .toString();
        }
    }

    static class Output implements Cloneable {

        /**
         * water level (depth) in the tank (h).
         */
        float level = 0;

        public Output(float level) {
            this.level = level;
        }

        public Output() {
        }

        @Override
        protected Output clone() {
            return new Output(level);
        }

        @Override
        public String toString() {
            return MoreObjects.toStringHelper("OUT")
                    .add("level", level)
                    .toString();
        }
    }

    static class ChartUI extends JFrame {

        private final static String TITLE = "Fluid level change in time";

        private XYDataset createDataset(List<ProbePoint> probes) {
            XYSeries seriesIn = new XYSeries("in");
            XYSeries seriesOut = new XYSeries("out");
            XYSeries seriesLevel = new XYSeries("level");
            XYSeries seriesSp = new XYSeries("SP");

            XYSeries seriesP = new XYSeries("P");
            XYSeries seriesI = new XYSeries("I");
            XYSeries seriesD = new XYSeries("D");


            for (ProbePoint point : probes) {
                int sample = point.sample;
                seriesIn.add(sample, point.in.flowIn);
                seriesOut.add(sample, point.in.flowOut);
                seriesLevel.add(sample, point.out.level);
                seriesSp.add(sample, point.setPoint.level);

                seriesP.add(sample, point.pidVal.valP);
                seriesI.add(sample, point.pidVal.valI);
                seriesD.add(sample, point.pidVal.valD);
            }
            XYSeriesCollection dataset = new XYSeriesCollection();
            dataset.addSeries(seriesIn);
            dataset.addSeries(seriesOut);
            dataset.addSeries(seriesLevel);
            dataset.addSeries(seriesSp);
            dataset.addSeries(seriesP);
            dataset.addSeries(seriesI);
            dataset.addSeries(seriesD);

            return dataset;
        }

        private JFreeChart createChart(XYDataset dataset) {

            JFreeChart chart = ChartFactory.createXYLineChart(
                    TITLE,
                    "Time",
                    "Level",
                    dataset,
                    PlotOrientation.VERTICAL,
                    true,
                    false,
                    false
            );

            AbstractXYItemRenderer renderer = new CyclicXYItemRenderer();

            renderer.setSeriesPaint(0, Color.RED);
            renderer.setSeriesStroke(0, createStroke());

            renderer.setSeriesPaint(1, Color.BLUE);
            renderer.setSeriesStroke(1, createStroke());

            renderer.setSeriesPaint(2, Color.GREEN);
            renderer.setSeriesStroke(2, createStroke());

            renderer.setSeriesPaint(3, Color.PINK);
            renderer.setSeriesStroke(3, createStroke());

            renderer.setSeriesPaint(4, Color.WHITE);
            renderer.setSeriesStroke(4, createStroke());

            renderer.setSeriesPaint(5, Color.BLACK);
            renderer.setSeriesStroke(5, createStroke());

            renderer.setSeriesPaint(6, Color.YELLOW);
            renderer.setSeriesStroke(6, createStroke());

            XYPlot plot = chart.getXYPlot();
            plot.setRenderer(renderer);
            plot.setBackgroundPaint(Color.GRAY);

            chart.getLegend().setFrame(BlockBorder.NONE);
            return chart;
        }

        private Stroke createStroke() {
            return new BasicStroke(2.0f);
        }

        private void createUI(List<ProbePoint> probes) {
            XYDataset dataset = createDataset(probes);
            JFreeChart chart = createChart(dataset);

            ChartPanel chartPanel = new ChartPanel(chart);
            chartPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            chartPanel.setBackground(Color.BLACK);
            add(chartPanel);

            pack();
            setTitle(TITLE);
            setLocationRelativeTo(null);
            setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }
}
